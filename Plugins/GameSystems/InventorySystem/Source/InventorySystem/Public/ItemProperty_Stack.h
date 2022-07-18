// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem.h"
#include "ItemProperty.h"

#include "ItemProperty_Stack.generated.h"

class UItemInstance;
struct FItemPropertyData_Stack;

/**
 * Item definition property to enable stacking of instances
 */
UCLASS()
class INVENTORYSYSTEM_API UItemProperty_Stack : public UItemProperty
{
	GENERATED_BODY()

public:
	/**
	 * The data type for this property class
	 */
	typedef FItemPropertyData_Stack FDataType;

	/**
	 * Constructor
	 */
	UItemProperty_Stack();

	/**
	 * Return the max stack count
	 */
	int64 GetMaxStackCount() const { return MaxStackCount; }

	/**
	 * Whether the two instances with the given property data can be stacked
	 */
	bool CanStack(const UItemInstance* Instance1, const UItemInstance* Instance2) const;

	/**
	 * Whether default new instances can be stacked to the given instance
	 *
	 * This is a shortcut for CanStack method, preventing actually creating a new instance when unnecessary
	 */
	bool CanStackDefault(const UItemInstance* Instance) const;

	// BEGIN UItemProperty interface
	virtual FItemPropertyData* CreateNewDataInstance() const override;
	// END UItemProperty interface

protected:
	/**
	 * Maximum number for stacking
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	int64 MaxStackCount;

	/**
	 * Whether default new instances can be stacked
	 *
	 * As of now, this does not check for the data stored in instance property data. This is useful for any items that
	 * do not have any mutable states in instances.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	bool bCanStackDefaultInstance;

	/**
	 * List of tags for comparing equality in the instance data
	 *
	 * Having a value of zero does not equal to not having an entry for the tag. If this array is empty, the instances
	 * are always considered stackable.
	 */
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Requirements")
	// TArray<TFieldPath<FProperty>> EqualityRequirements;

	// TODO Support more options for specifying item stack requirements
};

/**
 * Struct of data to store for each instance
 */
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItemPropertyData_Stack : public FItemPropertyData
{
	GENERATED_BODY()

	/**
	 * Maximum number for stacking, saved for easy access
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stack")
	int64 MaxStackCount = InventorySystem::GMaxStackCountDefault;

	// BEGIN FItemPropertyData interface
	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); }
	// END FItemPropertyData interface
};
