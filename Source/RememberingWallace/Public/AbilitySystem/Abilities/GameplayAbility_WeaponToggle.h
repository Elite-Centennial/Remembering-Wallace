// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityBase.h"

#include "GameplayAbility_WeaponToggle.generated.h"

class UAnimMontage;
class UItemProperty_Weapon;

/**
 * Base class for abilities for drawing/sheathing weapons
 */
UCLASS(Abstract)
class REMEMBERINGWALLACE_API UGameplayAbility_WeaponToggle : public UGameplayAbilityBase
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UGameplayAbility_WeaponToggle(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// BEGIN UGameplayAbility interface
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	// END UGameplayAbility interface

protected:
	/**
	 * Apply the weapon drawn tag Unit.State.Weapon.Drawn to the owning ASC
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Weapon")
	void ApplyWeaponDrawnTag();

	/**
	 * Remove the weapon drawn tag Unit.State.Weapon.Drawn from the owning ASC
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Weapon")
	void RemoveWeaponDrawnTag();

	/**
	 * Get the weapon item property for the currently equipped weapon
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability|Weapon")
	const UItemProperty_Weapon* GetCurrentWeaponProperty() const;

	/**
	 * Get the anim montage for drawing out the currently equipped weapon
	 *
	 * Returns true if the montage is set, false if not.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = "false", Category = "Ability|Weapon",
		meta = (ExpandBoolAsExecs = "ReturnValue", BlueprintProtected))
	bool GetDrawMontage(UAnimMontage*& DrawMontage) const;

	/**
	 * Get the anim montage for sheathing the currently equipped weapon
	 *
	 * Returns true if the montage is set, false if not.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = "false", Category = "Ability|Weapon",
		meta = (ExpandBoolAsExecs = "ReturnValue", BlueprintProtected))
	bool GetSheatheMontage(UAnimMontage*& SheatheMontage) const;
};
