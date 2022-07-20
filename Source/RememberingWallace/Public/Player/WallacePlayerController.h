// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InventorySystemInterface.h"
#include "GameFramework/PlayerController.h"

#include "WallacePlayerController.generated.h"

class AWallacePlayerState;
class UEquipmentManagerComponent;
class UWallaceAbilitySystemComponent;

/**
 * Customized player controller class for this project
 */
UCLASS()
class REMEMBERINGWALLACE_API AWallacePlayerController
	: public APlayerController, public IAbilitySystemInterface, public IInventorySystemInterface
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	AWallacePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Return the player state as the class used in this project
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controller")
	AWallacePlayerState* GetWallacePlayerState() const;

	/**
	 * Return the associated ASC from the player state as our customized class
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AbilitySystem")
	UWallaceAbilitySystemComponent* GetWallaceAbilitySystemComponent() const;

	/**
	 * Return the equipment manager component from the player state
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment")
	UEquipmentManagerComponent* GetEquipmentManagerComponent() const;

	// BEGIN IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// END IAbilitySystemInterface interface

	// BEGIN IInventorySystemInterface interface
	virtual UInventoryComponent* GetInventoryComponent_Implementation() const override;
	// END IInventorySystemInterface interface
};
