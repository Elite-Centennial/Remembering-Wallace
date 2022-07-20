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
	virtual UEquipmentManagerComponent* GetEquipmentManager() const { return nullptr; }

	/**
	 * Set if the character should have its rotation follow the controller's rotation
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetFollowControllerRotation(bool bFollowControllerRotation);

	// BEGIN IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// END IAbilitySystemInterface interface
};
