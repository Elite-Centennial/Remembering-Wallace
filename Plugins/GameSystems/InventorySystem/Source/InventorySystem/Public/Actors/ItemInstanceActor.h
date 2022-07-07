// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ItemInstanceActor.generated.h"

class UItemDefinition;
class UItemInstance;

/**
 * A manifestation of an item instance in the actual game world
 */
UCLASS()
class INVENTORYSYSTEM_API AItemInstanceActor : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	AItemInstanceActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Return the item instance this actor represents
	 */
	const UItemInstance* GetItemInstance() const { return ItemInstance; }

	/**
	 * Return the item instance this actor represents
	 */
	UItemInstance* GetItemInstance() { return ItemInstance; }

	/**
	 * Return the item definition stored in the item instance
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	const UItemDefinition* GetItemDefinition() const;

	/**
	 * Set the item instance of this actor, overriding the old value
	 *
	 * This does nothing when trying to assign the same instance.
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetItemInstance(UItemInstance* InItemInstance);

protected:
	/**
	 * Called when the associated item instance is changed
	 *
	 * The new and old values are guaranteed to be not equal, but either one could be null.
	 */
	virtual void OnItemInstanceSet(UItemInstance* NewValue, UItemInstance* OldValue) {}

	/**
	 * Called when the associated item instance is changed
	 *
	 * The new and old values are guaranteed to be not equal, but either one could be null.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Item", DisplayName = "On Item Instance Set")
	void BP_OnItemInstanceSet(UItemInstance* NewValue, UItemInstance* OldValue);

private:
	/**
	 * The item instance this actor represents
	 */
	// TODO: Detect when the item instance gets destroyed?
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess, ExposeOnSpawn))
	TObjectPtr<UItemInstance> ItemInstance;
};
