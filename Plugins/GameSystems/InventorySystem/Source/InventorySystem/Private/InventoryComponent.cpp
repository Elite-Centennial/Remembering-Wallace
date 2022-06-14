// Elite Centennial

#include "InventoryComponent.h"

#include "InventorySystemLog.h"
#include "ItemDefinition.h"

bool FInventorySlot::CanStackInstance(const UItemInstance* InstanceToStack, const uint32 CountToStack) const
{
	check(InstanceToStack);

	// Check the stack count first because checking the instance is expensive
	return CanStackAmount(CountToStack) && Instance->CanStackWith(InstanceToStack);
}

bool FInventorySlot::CanStackDefinition(const UItemDefinition* Definition, const uint32 CountToStack) const
{
	check(Definition);

	// Check the stack count first because checking the instance is expensive
	return CanStackAmount(CountToStack) && Instance->CanStackDefinition(Definition);
}

bool FInventorySlot::CanStackAmount(const uint32 CountToStack) const
{
	return CountToStack == 0 || Instance->GetMaxStackCount() - StackCount >= CountToStack;
}

FInventorySlot& FInventory::AddSlot(UItemInstance* Instance, const uint32 StackCount)
{
	check(Instance);

	AddSlotCount(Instance->GetDefinition()->GetIdentifierTag());
	return Slots.Emplace_GetRef(Instance, StackCount);
}

void FInventory::RemoveSlotAt(const uint32 IndexToRemove)
{
	if (static_cast<int64>(IndexToRemove) >= Slots.Num())
	{
		UE_LOG(LogInventory, Warning,
			TEXT("Tried to remove an out-of-bound inventory slot at [%d]"), IndexToRemove);
		return;
	}

	SubtractSlotCount(Slots[IndexToRemove].GetItemIdentifierTag());
	Slots.RemoveAt(IndexToRemove);
}

void FInventory::AddSlotCount(const FGameplayTag& ItemTag, const uint32 AmountToAdd)
{
	// Prevent the entry from being added if the amount to add is zero
	if (AmountToAdd == 0)
	{
		return;
	}

	// Automatically insert a new entry if the one for the given tag didn't exist
	uint32& Count = ItemSlotCount.FindOrAdd(ItemTag);
	Count += AmountToAdd;
}

bool FInventory::SubtractSlotCount(const FGameplayTag& ItemTag, const uint32 AmountToSubtract)
{
	// Don't do anything if the amount to subtract is zero
	if (AmountToSubtract == 0)
	{
		return true;
	}

	bool bHadEnoughCount = false;

	// Check if an entry exists for the given item tag
	if (uint32* CountPtr = ItemSlotCount.Find(ItemTag))
	{
		uint32& Count = *CountPtr;
		bHadEnoughCount = Count >= AmountToSubtract;
		Count -= FMath::Min(AmountToSubtract, Count);

		// Remove the entry if the resulting value is zero
		if (Count == 0)
		{
			// CAUTION: CountPtr and Count become invalid after the following line. Do NOT try to access those.
			ItemSlotCount.Remove(ItemTag);
		}
	}

	return bHadEnoughCount;
}

UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UInventoryComponent::HasInstance(const UItemInstance* Instance, uint32 Count) const
{
	if (Count == 0)
	{
		return true;
	}

	// Check existence
	if (!Inventory.HasItem(Instance->GetDefinition()))
	{
		return false;
	}

	// Go through the inventory array
	for (const FInventorySlot& Slot : Inventory.GetSlots())
	{
		// Skip slots that cannot stack the given item instance
		if (!Slot.CanStackInstance(Instance))
		{
			continue;
		}

		// If this slot has enough, return true
		if (Slot.StackCount >= Count)
		{
			return true;
		}

		// Subtract the existing amount and move on to the next slot
		Count -= Slot.StackCount;
	}

	// Logically, Count is positive here
	// Therefore, there are not enough items
	return false;
}

bool UInventoryComponent::BP_HasInstance(const UItemInstance* Instance, const int64 Count) const
{
	return Count >= 0 && HasInstance(Instance, Count);
}

bool UInventoryComponent::HasDefinition(const UItemDefinition* Definition, uint32 Count) const
{
	if (Count == 0)
	{
		return true;
	}

	// Check existence
	if (!Inventory.HasItem(Definition))
	{
		return false;
	}

	// Go through the inventory array
	for (const FInventorySlot& Slot : Inventory.GetSlots())
	{
		// Skip slots that cannot stack the given item definition
		if (!Slot.CanStackDefinition(Definition))
		{
			continue;
		}

		// If this slot has enough, return true
		if (Slot.StackCount >= Count)
		{
			return true;
		}

		// Subtract the existing amount and move on to the next slot
		Count -= Slot.StackCount;
	}

	// Logically, Count is positive here
	// Therefore, there are not enough items
	return false;
}

bool UInventoryComponent::BP_HasDefinition(const UItemDefinition* Definition, const int64 Count) const
{
	return Count >= 0 && HasDefinition(Definition, Count);
}

bool UInventoryComponent::HasItemTag(const FGameplayTag& ItemTag, uint32 Count) const
{
	if (Count == 0)
	{
		return true;
	}

	// Check existence
	if (!Inventory.HasItem(ItemTag))
	{
		return false;
	}

	// Go through the inventory array
	for (const FInventorySlot& Slot : Inventory.GetSlots())
	{
		// Skip slots with different item tags
		if (Slot.GetItemIdentifierTag() != ItemTag)
		{
			continue;
		}

		// If this slot has enough, return true
		if (Slot.StackCount >= Count)
		{
			return true;
		}

		// Subtract the existing amount and move on to the next slot
		Count -= Slot.StackCount;
	}

	// Logically, Count is positive here
	// Therefore, there are not enough items
	return false;
}

bool UInventoryComponent::BP_HasItemTag(const FGameplayTag& ItemTag, const int64 Count) const
{
	return Count >= 0 && HasItemTag(ItemTag, Count);
}

void UInventoryComponent::AddInstanceInit(const FItemInstanceInit& InstanceInit)
{
	Inventory.AddSlot(UItemInstance::CreateFromInstanceInit(InstanceInit, GetOwner()), InstanceInit.StackCount);
}

void UInventoryComponent::AddInstance(UItemInstance* Instance, uint32 Count)
{
	check(Instance);

	// In case zero is passed in, don't do anything
	if (Count == 0)
	{
		return;
	}

	const uint32 MaxStackCount = Instance->GetMaxStackCount();

	// Go through the inventory to add to the existing slots if stacking is supported and there is at least 1 slot with
	// the same item
	if (MaxStackCount > 1 && Inventory.HasItem(Instance->GetDefinition()))
	{
		// Iterate through inventory slots
		for (FInventorySlot& Slot : Inventory.GetSlots())
		{
			// If this slot cannot stack the instance, move on to the next slot
			if (!Slot.CanStackInstance(Instance))
			{
				continue;
			}

			// Save the instance to use
			// This is to minimize the number of item instance objects existing in the whole game instance and reuse as
			// many of those as possible. We try to enforce max 1 instance per 1 inventory for every stackable instance.
			Instance = Slot.Instance;

			// If the given amount can be stacked to this slot, do so and exit
			if (Slot.CanStackAmount(Count))
			{
				Slot.StackCount += Count;
				return;
			}

			// Fill in this slot and subtract the amount from the count that still needs to be added to the inventory
			Count -= MaxStackCount - Slot.StackCount;
			Slot.StackCount = MaxStackCount;

			// Move on to the next slot and repeat
		}
	}

	// Logically, Count should be positive here
	// Add extra slots to add the remaining amount

	// Change the outer object in case the instance came from another actor owner
	if (Instance->GetOuter() != GetOwner())
	{
		// TODO: experiment with the rename flags to see what they do and if we need those
		Instance->Rename(nullptr, GetOwner());
	}

	// Add new slots until the specified amount is all added
	while (Count > 0)
	{
		const FInventorySlot& Slot = Inventory.AddSlot(Instance, FMath::Min(Count, MaxStackCount));
		Count -= Slot.StackCount;
	}
}

void UInventoryComponent::BP_AddInstance(UItemInstance* Instance, const int64 Count)
{
	AddInstance(Instance, FMath::Max<int64>(Count, 0));
}

void UInventoryComponent::AddDefinition(const UItemDefinition* Definition, uint32 Count)
{
	check(Definition);

	// In case zero is passed in, don't do anything
	if (Count == 0)
	{
		return;
	}

	const uint32 MaxStackCount = Definition->GetMaxStackCount();
	UItemInstance* Instance = nullptr;

	// Go through the inventory to add to the existing slots if stacking is supported and there is at least 1 slot with
	// the same item
	if (MaxStackCount > 1 && Inventory.HasItem(Definition))
	{
		// Iterate through inventory slots
		for (FInventorySlot& Slot : Inventory.GetSlots())
		{
			// If this slot cannot stack the definition, move on to the next slot
			if (!Slot.CanStackDefinition(Definition))
			{
				continue;
			}

			// Save the instance to use
			Instance = Slot.Instance.Get();

			// If the given amount can be stacked to this slot, do so and exit
			if (Slot.CanStackAmount(Count))
			{
				Slot.StackCount += Count;
				return;
			}

			// Fill in this slot and subtract the amount from the count that still needs to be added to the inventory
			Count -= MaxStackCount - Slot.StackCount;
			Slot.StackCount = MaxStackCount;

			// Move on to the next slot and repeat
		}
	}

	// Logically, Count should be positive here
	// Add extra slots to add the remaining amount

	// If no existing instance was found to use, create a new one
	if (!Instance)
	{
		Instance = UItemInstance::CreateFromDefinition(Definition, GetOwner());
	}

	// Add new slots until the specified amount is all added
	while (Count > 0)
	{
		const FInventorySlot& Slot = Inventory.AddSlot(Instance, FMath::Min(Count, MaxStackCount));
		Count -= Slot.StackCount;
	}
}

void UInventoryComponent::BP_AddDefinition(const UItemDefinition* Definition, const int64 Count)
{
	AddDefinition(Definition, FMath::Max<int64>(Count, 0));
}

UItemInstance* UInventoryComponent::RemoveInstance(const UItemInstance* Instance, uint32 Count)
{
	if (Count == 0)
	{
		return nullptr;
	}

	TArray<FInventorySlot>& Slots = Inventory.GetSlots();
	UItemInstance* RemovedInstance = nullptr;

	// Go through the list, starting from the end, so that we do not get affected by any removals of slots in the array
	for (auto Index = Slots.Num() - 1; Index >= 0; Index--)
	{
		FInventorySlot& Slot = Slots[Index];

		if (!Slot.CanStackInstance(Instance))
		{
			continue;
		}

		if (!RemovedInstance)
		{
			RemovedInstance = Slot.Instance.Get();
		}

		if (Count >= Slot.StackCount)
		{
			Count -= Slot.StackCount;
			Inventory.RemoveSlotAt(Index);
		}
		else
		{
			Slot.StackCount -= Count;
			Count = 0;
		}

		// Exit early if all amounts are removed
		if (Count == 0)
		{
			return RemovedInstance;
		}
	}

	return RemovedInstance;
}

UItemInstance* UInventoryComponent::BP_RemoveInstance(const UItemInstance* Instance, const int64 Count)
{
	return RemoveInstance(Instance, FMath::Max<int64>(Count, 0));
}

UItemInstance* UInventoryComponent::RemoveDefinition(const UItemDefinition* Definition, uint32 Count)
{
	if (Count == 0)
	{
		return nullptr;
	}

	TArray<FInventorySlot>& Slots = Inventory.GetSlots();
	UItemInstance* RemovedInstance = nullptr;

	// Go through the list, starting from the end, so that we do not get affected by any removals of slots in the array
	for (auto Index = Slots.Num() - 1; Index >= 0; Index--)
	{
		FInventorySlot& Slot = Slots[Index];

		if (!Slot.CanStackDefinition(Definition))
		{
			continue;
		}

		if (!RemovedInstance)
		{
			RemovedInstance = Slot.Instance.Get();
		}

		if (Count >= Slot.StackCount)
		{
			Count -= Slot.StackCount;
			Inventory.RemoveSlotAt(Index);
		}
		else
		{
			Slot.StackCount -= Count;
			Count = 0;
		}

		// Exit early if all amounts are removed
		if (Count == 0)
		{
			return RemovedInstance;
		}
	}

	return RemovedInstance;
}

UItemInstance* UInventoryComponent::BP_RemoveDefinition(const UItemDefinition* Definition, const int64 Count)
{
	return RemoveDefinition(Definition, FMath::Max<int64>(Count, 0));
}

FInventorySlot UInventoryComponent::RemoveSlotAt(const uint32 IndexToRemove)
{
	if (static_cast<int64>(IndexToRemove) >= Inventory.GetSlots().Num())
	{
		return FInventorySlot();
	}

	FInventorySlot Slot = Inventory.GetSlots()[IndexToRemove];
	Inventory.RemoveSlotAt(IndexToRemove);
	return Slot;
}

FInventorySlot UInventoryComponent::BP_RemoveSlotAt(const int64 Index)
{
	return Index >= 0 ? RemoveSlotAt(Index) : FInventorySlot();
}
