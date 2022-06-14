// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "ItemProperty.generated.h"

class UItemInstance;
struct FItemPropertyData;

/**
 * Base class of a property (a set of values) for an item definition
 *
 * To add new kinds of values for an item, create a subclass of this class and add it to the item definition asset. You
 * MUST do 2 things in the child class if that child class is written in C++:
 * 1. Create a public typedef of FDataType. The type is the struct that is used for saving item instance data for this
 *    property. If the property class does not need any data saved, then use FItemPropertyData provided by this header
 *    file.
 * 2. Override CreateNewDataInstance method to allocate memory for the type you assigned to FDataType typedef. If the
 *    property class does not need any data saved to item instances, then you do not need to override this.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UItemProperty : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * The data type for this property class
	 *
	 * Every non-abstract child class MUST also define their own typedef for this.
	 */
	typedef FItemPropertyData FDataType;

	/**
	 * Constructor
	 */
	UItemProperty(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Called when a new item instance is created from the owning item definition
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	void OnInstanceCreated(UItemInstance* Instance) const;

	/**
	 * Create a new instance of the data struct with the appropriate struct class
	 *
	 * The default implementation does not allocate any memory and just returns nullptr.
	 */
	virtual FItemPropertyData* CreateNewDataInstance() const;
};

/**
 * Base struct class to use as a place to save data for item definition properties
 *
 * To define a new struct type to store item instance data, GetScriptStruct method MUST be overriden to return the
 * appropriate value.
 */
// TODO: Change FItemPropertyData to UObject?
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItemPropertyData
{
	GENERATED_BODY()

	/**
	 * Virtual destructor in case any child structs need overriding
	 */
	virtual ~FItemPropertyData() = default;

	/**
	 * Return the real struct type for the instance
	 *
	 * All child structs MUST override this method and return the appropriate value.
	 */
	virtual UScriptStruct* GetScriptStruct() const { return StaticStruct(); }
};
