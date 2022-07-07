// Elite Centennial

#include "InventoryComponent.h"

#include "InventorySystemLog.h"
#include "ItemDefinition.h"

FInventorySlot::FInventorySlot(UItemInstance* InInstance, const int64 InStackCount)
	: Instance(InInstance),
	  StackCount(FMath::Max<int64>(InStackCount, 0))
{
}

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

FInventorySlot& FInventory::AddSlot(UItemInstance* Instance, const int64 StackCount)
{
	check(Instance);

	AddSlotCount(Instance->GetDefinition()->GetIdentifierTag());
	return Slots.Emplace_GetRef(Instance, StackCount);
}

void FInventory::RemoveSlotAt(const int32 IndexToRemove)
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

void FInventory::AddSlotCount(const FGameplayTag& ItemTag, const uint16 AmountToAdd)
{
	// Prevent the entry from being added if the amount to add is zero
	if (AmountToAdd == 0)
	{
		return;
	}

	// Automatically insert a new entry if the one for the given tag didn't exist
	uint16& Count = ItemSlotCount.FindOrAdd(ItemTag);
	Count += AmountToAdd;
}

bool FInventory::SubtractSlotCount(const FGameplayTag& ItemTag, const uint16 AmountToSubtract)
{
	// Don't do anything if the amount to subtract is zero
	if (AmountToSubtract == 0)
	{
		return true;
	}

	bool bHadEnoughCount = false;

	// Check if an entry exists for the given item tag
	if (uint16* CountPtr = ItemSlotCount.Find(ItemTag))
	{
		uint16& Count = *CountPtr;
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

bool UInventoryComponent::HasInstance(const UItemInstance* Instance, int64 Count) const
{
	check(Instance);

	// Check existence
	if (Count < 0 || !Inventory.HasItem(Instance->GetDefinition()))
	{
		return false;
	}

	// Existence is checked; return true if Count is 0 or 1
	if (Count < 2)
	{
		return true;
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

bool UInventoryComponent::HasDefinition(const UItemDefinition* Definition, int64 Count) const
{
	check(Definition);

	// Check existence
	if (Count < 0 || !Inventory.HasItem(Definition))
	{
		return false;
	}

	// Existence is checked; return true if Count is 0 or 1
	if (Count < 2)
	{
		return true;
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

bool UInventoryComponent::HasItemTag(const FGameplayTag& ItemTag, int64 Count) const
{
	check(ItemTag.IsValid());

	// Check existence
	if (Count < 0 || !Inventory.HasItem(ItemTag))
	{
		return false;
	}

	// Existence is checked; return true if Count is 0 or 1
	if (Count < 2)
	{
		return true;
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

void UInventoryComponent::AddInstanceInit(const FItemInstanceInit& InstanceInit)
{
	AddInstance(UItemInstance::CreateFromInstanceInit(InstanceInit, GetOwner()), InstanceInit.StackCount);
}

void UInventoryComponent::AddInstance(UItemInstance* Instance, int64 Count)
{
	check(Instance);

	// In case a non-positive value is passed in, don't do anything
	if (Count <= 0)
	{
		return;
	}

	const int64 MaxStackCount = Instance->GetMaxStackCount();

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

void UInventoryComponent::AddDefinition(const UItemDefinition* Definition, int64 Count)
{
	check(Definition);

	// In case a non-positive value is passed in, don't do anything
	if (Count <= 0)
	{
		return;
	}

	const int64 MaxStackCount = Definition->GetMaxStackCount();
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

UItemInstance* UInventoryComponent::RemoveInstance(const UItemInstance* Instance, int64 Count)
{
	check(Instance);

	if (Count <= 0)
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

// TODO: Create a new item instance for the removed items? What should we assign to the outer UObject of that?
UItemInstance* UInventoryComponent::RemoveDefinition(const UItemDefinition* Definition, int64 Count)
{
	check(Definition);

	if (Count <= 0)
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

FInventorySlot UInventoryComponent::RemoveSlotAt(const int32 Index)
{
	// Ensure the index is not out of bounds of the array
	if (Index < 0 || Index >= Inventory.GetSlots().Num())
	{
		return FInventorySlot();
	}

	FInventorySlot Slot = Inventory.GetSlots()[Index];
	Inventory.RemoveSlotAt(Index);
	return Slot;
}
