// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InventorySystemInterface.h"
#include "GameFramework/Character.h"

#include "CharacterBase.generated.h"

class ACharacterBase;
class UCharacterBaseMovementComponent;
class UEquipmentManagerComponent;
class UWallaceAbilitySystemComponent;

/**
 * Abstract base character class for all character classes
 *
 * This class mostly consists of common functionalities such as the custom character movement component.
 */
UCLASS(Abstract)
class REMEMBERINGWALLACE_API ACharacterBase
	: public ACharacter, public IAbilitySystemInterface, public IInventorySystemInterface
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	ACharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Return the customized character movement component
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	UCharacterBaseMovementComponent* GetCharacterBaseMovement() const;

	/**
	 * Return the ASC of this unit as the customized type for this project
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AbilitySystem")
	virtual UWallaceAbilitySystemComponent* GetWallaceAbilitySystemComponent() const { return nullptr; }

	/**
	 * Return the equipment manager of this unit
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment")
	virtual UEquipmentManagerComponent* GetEquipmentManagerComponent() const { return nullptr; }

	/**
	 * Set if the character should have its rotation follow the controller's rotation
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetFollowControllerRotation(bool bFollowControllerRotation);

	/**
	 * Handle the event where the weapon is grabbed while drawing it out
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void HandleWeaponGrabbed();

	/**
	 * Handle the event where the weapon is released while sheathing it
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void HandleWeaponReleased();

	// BEGIN IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// END IAbilitySystemInterface interface

	// BEGIN IInventorySystemInterface interface
	// virtual UInventoryComponent* GetInventoryComponent_Implementation() const override { return nullptr; }
	// END IInventorySystemInterface interface

protected:
	/**
	 * Called when the weapon should be grabbed while drawing it out
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnWeaponGrabbed();

	/**
	* Called when the weapon should be release while sheathing it
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnWeaponReleased();
};
