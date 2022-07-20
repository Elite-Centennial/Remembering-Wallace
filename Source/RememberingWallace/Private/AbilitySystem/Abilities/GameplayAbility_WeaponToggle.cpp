// Elite Centennial

#include "AbilitySystem/Abilities/GameplayAbility_WeaponToggle.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "ItemInstance.h"
#include "RememberingWallaceTags.h"
#include "Character/CharacterBase.h"
#include "Items/EquipmentManagerComponent.h"
#include "Items/ItemProperty_Weapon.h"

UGameplayAbility_WeaponToggle::UGameplayAbility_WeaponToggle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Important! This must NOT be set to NonInstanced!
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// All child classes have this tag by default
	AbilityTags.AddTag(TAG(Ability.Action.Weapon));

	// Block other abilities of same type (i.e., change the weapon state)
	BlockAbilitiesWithTag.AddTag(TAG(Ability.Action.Weapon));
}

bool UGameplayAbility_WeaponToggle::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	// In addition to the checks done in the parent implementation, we check if
	// 1. the avatar actor is valid and is of ACharacterBase class
	// 2. the owner currently has a weapon equipped

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// Avatar actor is not null because of the parent implementation
	const ACharacterBase* Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());

	// 1. Check if the character avatar is of type ACharacterBase
	if (!Character)
	{
		return false;
	}

	const UEquipmentManagerComponent* EquipmentManager = Character->GetEquipmentManager();

	// 2. Check if we have a weapon equipped
	if (!EquipmentManager || !EquipmentManager->HasWeaponEquipped())
	{
		return false;
	}

	// All checks passed
	return true;
}

void UGameplayAbility_WeaponToggle::ApplyWeaponDrawnTag()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Checked();
	AbilitySystemComponent->SetLooseGameplayTagCount(TAG(Weapon.State.Drawn), 1);
}

void UGameplayAbility_WeaponToggle::RemoveWeaponDrawnTag()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Checked();
	AbilitySystemComponent->SetLooseGameplayTagCount(TAG(Weapon.State.Drawn), 0);
}

const UItemProperty_Weapon* UGameplayAbility_WeaponToggle::GetCurrentWeaponProperty() const
{
	const UEquipmentManagerComponent* EquipmentManager = GetEquipmentManagerFromActorInfo();
	check(EquipmentManager); // Guaranteed by CanActivateAbility override
	const UItemInstance* CurrentWeapon = EquipmentManager->GetCurrentWeapon();
	check(CurrentWeapon); // Guaranteed by CanActivateAbility override
	const UItemProperty_Weapon* WeaponProperty = CurrentWeapon->GetWeaponProperty();
	check(WeaponProperty); // Guaranteed by UEquipmentManagerComponent::EquipWeapon
	return WeaponProperty;
}

bool UGameplayAbility_WeaponToggle::GetDrawMontage(UAnimMontage*& DrawMontage) const
{
	DrawMontage = GetCurrentWeaponProperty()->GetDrawMontage();
	return DrawMontage != nullptr;
}

bool UGameplayAbility_WeaponToggle::GetSheatheMontage(UAnimMontage*& SheatheMontage) const
{
	SheatheMontage = GetCurrentWeaponProperty()->GetSheatheMontage();
	return SheatheMontage != nullptr;
}
