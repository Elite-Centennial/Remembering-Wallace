// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "ItemInstance.h"
#include "Components/ActorComponent.h"

#include "InventoryComponent.generated.h"

class UInventoryComponent;

/**
 * A slot/stack in the inventory
 */
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FInventorySlot
{
	GENERATED_BODY()

	/**
	 * Convenience constructor
	 */
	explicit FInventorySlot(UItemInstance* InInstance = nullptr, const int64 InStackCount = 0);

	/**
	 * Copy constructor
	 */
	FInventorySlot(const FInventorySlot& Other) : Instance(Other.Instance), StackCount(Other.StackCount)
	{
	}

	/**
	 * The item instance for this slot
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UItemInstance> Instance;

	/**
	 * The number of items stacked in this slot
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stack")
	int64 StackCount;

	/**
	 * Whether ths data is in a valid state (having an item instance assigned and a positive stack count)
	 */
	bool IsValid() const { return Instance.Get() && StackCount > 0; }

	/**
	 * Utility method to return item identifier tag stored in the item definition
	 */
	const FGameplayTag& GetItemIdentifierTag() const { return Instance->GetDefinition()->GetIdentifierTag(); }

	/**
	 * Whether the given item instance can be stacked to this slot
	 */
	bool CanStackInstance(const UItemInstance* InstanceToStack, uint32 CountToStack = 0) const;

	/**
	 * Whether the given item definition can be stacked to this slot
	 */
	bool CanStackDefinition(const UItemDefinition* Definition, uint32 CountToStack = 0) const;

	/**
	 * Whether this slot has space to stack the given amount
	 */
	bool CanStackAmount(uint32 CountToStack) const;
};

/**
 * Wrapper/container type for the inventory
 */
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FInventory
{
	GENERATED_BODY()

	/**
	 * Whether the inventory contains any items of the given item type
	 */
	bool HasItem(const FGameplayTag& ItemTag) const { return ItemSlotCount.Contains(ItemTag); }

	/**
	 * Whether the inventory contains any items of the given item type
	 */
	bool HasItem(const UItemDefinition* ItemDefinition) const { return HasItem(ItemDefinition->GetIdentifierTag()); }

	/**
	 * Return the inventory slots array
	 */
	const TArray<FInventorySlot>& GetSlots() const { return Slots; }

	/**
	 * Return the inventory slots array
	 */
	TArray<FInventorySlot>& GetSlots() { return Slots; }

	/**
	 * Add a slot to the inventory
	 */
	FInventorySlot& AddSlot(UItemInstance* Instance, int64 StackCount);

	/**
	 * Remove a slot at the given index of the array
	 */
	void RemoveSlotAt(int32 IndexToRemove);

protected:
	/**
	 * List of items in this inventory
	 *
	 * Do NOT directly modify this field.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> Slots;

	// TODO: More performant way to organize the inventory

	/**
	 * Cached map of how many slots each item type is taking up in the inventory
	 */
	TMap<FGameplayTag, uint16> ItemSlotCount;

private:
	/**
	 * Add the given amount to the number of slots saved in ItemSlotCount
	 */
	void AddSlotCount(const FGameplayTag& ItemTag, uint16 AmountToAdd = 1);

	/**
	 * Subtract the given amount from the number of slots saved in ItemSlotCount
	 *
	 * Returns false if there was not enough amount to fully subtract the given amount, but the subtraction still takes
	 * place.
	 */
	bool SubtractSlotCount(const FGameplayTag& ItemTag, uint16 AmountToSubtract = 1);
};

/**
 * Actor component responsible for managing an inventory
 */
UCLASS(BlueprintType, ClassGroup = "Gameplay", meta = (BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Get a non-mutable reference to the inventory list
	 *
	 * Do NOT directly modify the returned array.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	const TArray<FInventorySlot>& GetInventory() const { return Inventory.GetSlots(); }

	/**
	 * Whether the inventory contains the given amount of items that can be stacked with the given item instance
	 *
	 * Specifying 0 or 1 for Count results in only checking the existence. A negative Count value results in returning
	 * false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool HasInstance(const UItemInstance* Instance, int64 Count = 1) const;

	/**
	 * Whether the inventory contains the given amount of items that can be stacked with the given item definition
	 *
	 * Specifying 0 or 1 for Count results in only checking the existence. A negative Count value results in returning
	 * false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool HasDefinition(const UItemDefinition* Definition, int64 Count = 1) const;

	/**
	 * Whether the inventory contains the given amount of items with the given item tag
	 *
	 * Specifying 0 or 1 for Count results in only checking the existence. A negative Count value results in returning
	 * false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool HasItemTag(const FGameplayTag& ItemTag, int64 Count = 1) const;

	/**
	 * Add item from an item instance init info
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddInstanceInit(const FItemInstanceInit& InstanceInit);

	/**
	 * Add an item instance to the inventory
	 *
	 * A non-positive value for Count is ignored, and nothing happens in that case.
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddInstance(UItemInstance* Instance, int64 Count = 1);

	/**
	 * Add an item to the inventory using its definition
	 *
	 * A non-positive value for Count is ignored, and nothing happens in that case.
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddDefinition(const UItemDefinition* Definition, int64 Count = 1);

	/**
	 * Remove an item from the inventory that matches the given item instance
	 *
	 * Returns the removed instance. This removes the items that can stack with the given item instance. A non-positive
	 * value for Count is ignored, and nothing happens in that case.
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UItemInstance* RemoveInstance(const UItemInstance* Instance, int64 Count = 1);

	/**
	 * Remove an item from the inventory that matches the given item definition
	 *
	 * Returns the removed instance. This only removes the ones that can stack with the definition. If you want to
	 * remove items that are associated with specific item instances, use RemoveInstance instead. A non-positive value
	 * for Count is ignored, and nothing happens in that case.
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UItemInstance* RemoveDefinition(const UItemDefinition* Definition, int64 Count = 1);

	/**
	 * Remove an inventory slot at the given index in the array
	 *
	 * Returns the removed inventory slot. If the index is out of bounds, an empty and invalid inventory slot is
	 * returned.
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FInventorySlot RemoveSlotAt(int32 Index);

protected:
	/**
	 * The inventory container
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Inventory")
	FInventory Inventory;
};
