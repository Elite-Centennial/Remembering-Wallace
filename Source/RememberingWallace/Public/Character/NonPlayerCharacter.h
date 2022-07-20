// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "InventorySystemInterface.h"

#include "NonPlayerCharacter.generated.h"

class UEquipmentManagerComponent;
class UInventoryComponent;
class UWallaceAbilitySystemComponent;

/**
 * Character class for non-player units like townspeople or enemies
 */
UCLASS()
class REMEMBERINGWALLACE_API ANonPlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Cached names for the components
	static const FName AbilitySystemComponentName;
	static const FName InventoryComponentName;
	static const FName EquipmentManagerComponentName;

	/**
	 * Constructor
	 */
	ANonPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// BEGIN IInventorySystemInterface interface
	virtual UInventoryComponent* GetInventoryComponent_Implementation() const override { return InventoryComponent; }
	// END IInventorySystemInterface interface

	// BEGIN ACharacterBase interface
	virtual UWallaceAbilitySystemComponent* GetWallaceAbilitySystemComponent() const override { return AbilitySystemComponent; }
	virtual UEquipmentManagerComponent* GetEquipmentManager() const override { return EquipmentManagerComponent; }
	// END ACharacterBase interface

	// BEGIN APawn interface
	virtual void PossessedBy(AController* NewController) override;
	// END APawn interface

private:
	/**
	 * The ASC for this unit
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess))
	TObjectPtr<UWallaceAbilitySystemComponent> AbilitySystemComponent;

	/**
	 * The inventory for this unit
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess))
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/**
	 * Equipment manager for this unit
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess))
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;
};
