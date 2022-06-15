// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "ItemProperty.h"
#include "UObject/Object.h"

#include "ItemInstance.generated.h"

class UInventoryComponent;
class UItemDefinition;

/**
 * Data for manually instantiating items
 *
 * This is useful for manually initializing inventories or loading inventories from saves.
 */
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItemInstanceInit
{
	GENERATED_BODY()

	/**
	 * Item definition to use to instantiate the item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UItemDefinition> Definition = nullptr;

	/**
	 * The stack count if the item is stackable in inventories
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stack", meta = (ClampMin = "1", UIMin = "1"))
	int64 StackCount = 1;
};

/**
 * An instance of an item
 */
UCLASS(BlueprintType)
class INVENTORYSYSTEM_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Create a new instance from the given item definition and the outer UObject
	 */
	static UItemInstance* CreateFromDefinition(const UItemDefinition* Definition, UObject* Outer);

	/**
	 * Create a new instance from the given instance init and the outer UObject
	 */
	static UItemInstance* CreateFromInstanceInit(const FItemInstanceInit& InstanceInit, UObject* Outer);

	/**
	 * Return a non-mutable reference to the item definition
	 */
	const UItemDefinition* GetDefinition() const { return Definition; }

	/**
	 * Return the instance data for the given property class
	 *
	 * Do not persist the returned pointer. The memory might be reallocated or freed throughout the lifetime of the
	 * object instance.
	 */
	const FItemPropertyData* GetInstanceData(TSubclassOf<UItemProperty> PropertyClass) const;

	/**
	 * Return the instance data for the given property class
	 *
	 * Do not persist the returned pointer. The memory might be reallocated or freed throughout the lifetime of the
	 * object instance.
	 */
	FItemPropertyData* GetInstanceData(TSubclassOf<UItemProperty> PropertyClass);

	/**
	 * Return the instance data for the given property class
	 */
	template<class T>
	const typename T::FDataType* GetInstanceData() const
	{
		return static_cast<const typename T::FDataType*>(GetInstanceData(T::StaticClass()));
	}

	/**
	 * Return the instance data for the given property class
	 */
	template<class T>
	typename T::FDataType* GetInstanceData()
	{
		return static_cast<typename T::FDataType*>(GetInstanceData(T::StaticClass()));
	}

	/**
	 * Utility method that returns maximum stack count stored in the property data
	 *
	 * If stacking is not supported, this returns 1.
	 */
	int64 GetMaxStackCount() const;

	/**
	 * Whether this instance can be stacked with the given instance
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool CanStackWith(const UItemInstance* Other) const;

	/**
	 * Whether this instance can stack with a default new instance for the given item definition
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool CanStackDefinition(const UItemDefinition* OtherDefinition) const;

protected:
	/**
	 * Item definition associated with this instance
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UItemDefinition> Definition;

	/**
	 * Data for this instance stored by item definition properties
	 *
	 * The key is the name of the property class. The values are initialized by the method
	 * UItemProperty::CreateNewDataInstance.
	 */
	TMap<FName, TUniquePtr<FItemPropertyData>> InstanceData;

private:
	/**
	 * Initialize the data from an item definition
	 *
	 * This is intended to be called right after creating a new instance of this class with NewObject function. Do NOT
	 * call this method anywhere else.
	 */
	void InitializeFromDefinition(const UItemDefinition* InDefinition);

	/**
	 * Initialize the data from an item instance init
	 */
	void InitializeFromInstanceInit(const FItemInstanceInit& InstanceInit);
};
