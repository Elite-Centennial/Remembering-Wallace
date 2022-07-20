// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "GameplayAbilityBase.generated.h"

class ACharacterBase;
class UEquipmentManagerComponent;
class UInventoryComponent;
class UWallaceAbilitySystemComponent;

/**
 * The base class of gameplay abilities used in this project
 */
UCLASS(Abstract, HideCategories = "Input")
class REMEMBERINGWALLACE_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UGameplayAbilityBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Whether this ability can be automatically activated from player input
	 */
	bool ShouldActivateOnInput() const { return bActivateOnInput; }

	/**
	 * Get the character actor associated with this ability instance
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	ACharacterBase* GetCharacterFromActorInfo() const;

	/**
	 * Get our customized ASC associated with this ability instance
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	UWallaceAbilitySystemComponent* GetWallaceAbilitySystemComponentFromActorInfo() const;

	/**
	 * Get the inventory component associated with this ability instance
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	UInventoryComponent* GetInventoryComponentFromActorInfo() const;

	/**
	 * Get the equipment manager component associated with this ability instance
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	UEquipmentManagerComponent* GetEquipmentManagerFromActorInfo() const;

protected:
	/**
	 * Whether this ability can be automatically activated from player input if bound to an input action
	 *
	 * Input pressed/released events are still sent even if this is set to false.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Triggers")
	bool bActivateOnInput;
};
