// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InventorySystemInterface.h"
#include "GameFramework/PlayerState.h"

#include "WallacePlayerState.generated.h"

class UEquipmentManagerComponent;
class UInventoryComponent;
class UWallaceAbilitySystemComponent;

/**
 * The player state class that is used in this project
 */
UCLASS()
class REMEMBERINGWALLACE_API AWallacePlayerState
	: public APlayerState, public IAbilitySystemInterface, public IInventorySystemInterface
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
	AWallacePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Return the ASC as the customized type for this project
	 */
	UWallaceAbilitySystemComponent* GetWallaceAbilitySystemComponent() const { return AbilitySystemComponent; }

	/**
	 * Return the equipment manager component
	 */
	UEquipmentManagerComponent* GetEquipmentManagerComponent() const { return EquipmentManagerComponent; }

	// BEGIN IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// END IAbilitySystemInterface interface

	// BEGIN IInventorySystemInterface interface
	virtual UInventoryComponent* GetInventoryComponent_Implementation() const override { return InventoryComponent; }
	// END IInventorySystemInterface interface

private:
	/**
	 * The ASC for the player
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess))
	TObjectPtr<UWallaceAbilitySystemComponent> AbilitySystemComponent;

	/**
	 * The inventory for the player
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess))
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/**
	 * Equipment manager for the player
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess))
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;
};
