// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"

#include "ItemDefinition.generated.h"

class UItemProperty;

/**
 * Non-mutable information of an item
 *
 * A definition of an item is composed of properties. Each property defines a collection of parameters and the behaviour
 * of the item. Whenever an item is spawned in the game world (either as an actor or just as an entry in an inventory),
 * it is "instantiated" as an item instance. This way, we can have multiple items of same type performing individual
 * actions instead of having a shared state and interfering with each other.
 */
UCLASS(BlueprintType, Const)
class INVENTORYSYSTEM_API UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Return the gameplay tag used for identifying the item
	 */
	const FGameplayTag& GetIdentifierTag() const { return IdentifierTag; }

	/**
	 * Return the display name for this item
	 */
	const FText& GetDisplayName() const { return DisplayName; }

	/**
	 * Return the contained property of the specified type
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item", DisplayName = "Get Item Property",
		meta = (DeterminesOutputType = "PropertyClass"))
	const UItemProperty* GetProperty(TSubclassOf<UItemProperty> PropertyClass) const;

	/**
	 * Return the contained property of the specified type
	 */
	template<class T>
	const T* GetProperty() const { return Cast<T>(GetProperty(T::StaticClass())); }

	/**
	 * Return a non-mutable reference to the properties
	 */
	const TArray<TObjectPtr<UItemProperty>>& GetAllProperties() const { return Properties; }

	/**
	 * Return the maximum number of amount the item can be stacked in one inventory slot
	 */
	int64 GetMaxStackCount() const;

protected:
	/**
	 * The unique identifiable tag associated with this item
	 *
	 * We use this when doing equality checks on item types. The tags must take the form of "ItemDef.Item.<Something>"
	 * where the "Something" can be several levels of child tags.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseProperty", meta = (Categories = "ItemDef.Item"))
	FGameplayTag IdentifierTag;

	/**
	 * Name of this item that is displayed to the player
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseProperty")
	FText DisplayName;

	/**
	 * Properties associated with this item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties", Instanced)
	TArray<TObjectPtr<UItemProperty>> Properties;
};
