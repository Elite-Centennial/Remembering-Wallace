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
	 * Return the item property contained in the item definition of the specified type
	 *
	 * This is a simple wrapper to calling ItemInstance->GetDefinition()->GetProperty<PropertyClass>().
	 */
	template<class T>
	const T* GetProperty() const;

	/**
	 * Return the item property contained in the item definition of the specified type
	 *
	 * This is a simple wrapper to calling ItemInstance->GetDefinition()->GetProperty(PropertyClass).
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item", DisplayName = "Get Item Property",
		meta = (DeterminesOutputType = "PropertyClass"))
	const UItemProperty* GetProperty(TSubclassOf<UItemProperty> PropertyClass) const;

	/**
	 * Return the instance data for the given property class
	 *
	 * Do not persist the returned pointer. The memory might be reallocated or freed throughout the lifetime of the
	 * object instance.
	 */
	template<class T>
	const typename T::FDataType* GetPropertyData() const;

	/**
	 * Return the instance data for the given property class
	 *
	 * Do not persist the returned pointer. The memory might be reallocated or freed throughout the lifetime of the
	 * object instance.
	 */
	template<class T>
	typename T::FDataType* GetPropertyData();

	/**
	 * Return the instance data for the given property class
	 *
	 * Do not persist the returned pointer. The memory might be reallocated or freed throughout the lifetime of the
	 * object instance.
	 */
	const FItemPropertyData* GetPropertyData(TSubclassOf<UItemProperty> PropertyClass) const;

	/**
	 * Return the instance data for the given property class
	 *
	 * Do not persist the returned pointer. The memory might be reallocated or freed throughout the lifetime of the
	 * object instance.
	 */
	FItemPropertyData* GetPropertyData(TSubclassOf<UItemProperty> PropertyClass);

	/**
	 * Utility method that returns maximum stack count stored in the property data
	 *
	 * If stacking is not supported, this returns 1.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Stack")
	int64 GetMaxStackCount() const;

	/**
	 * Whether this instance can be stacked with the given instance
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Stack")
	bool CanStackWith(const UItemInstance* Other) const;

	/**
	 * Whether this instance can stack with a default new instance for the given item definition
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Stack")
	bool CanStackDefinition(const UItemDefinition* OtherDefinition) const;

protected:
	/**
	 * Item definition associated with this instance
	 */
	// TODO: Change to const pointer
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UItemDefinition> Definition;

	/**
	 * Data for this instance stored by item definition properties
	 *
	 * The key is the name of the property class. The values are initialized by the method
	 * UItemProperty::CreateNewDataInstance.
	 */
	// TODO: Change this to a "handle" pattern which GAS uses a lot; this will allow easier integration with blueprint
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

template<class T>
const T* UItemInstance::GetProperty() const
{
	return CastChecked<T>(GetProperty(T::StaticClass()), ECastCheckedType::NullAllowed);
}

template<class T>
const typename T::FDataType* UItemInstance::GetPropertyData() const
{
	const FItemPropertyData* Data = GetPropertyData(T::StaticClass());
	if (Data)
	{
		check(Data->GetScriptStruct() == T::FDataType::StaticStruct());
	}
	return static_cast<const typename T::FDataType*>(Data);
}

template<class T>
typename T::FDataType* UItemInstance::GetPropertyData()
{
	FItemPropertyData* Data = GetPropertyData(T::StaticClass());
	if (Data)
	{
		check(Data->GetScriptStruct() == T::FDataType::StaticStruct());
	}
	return static_cast<typename T::FDataType*>(Data);
}
