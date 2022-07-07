// Elite Centennial

#include "AbilitySystem/Abilities/GameplayAbility_WeaponToggle.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "ItemInstance.h"
#include "Character/CharacterBase.h"
#include "Items/EquipmentManagerComponent.h"
#include "Items/ItemProperty_Weapon.h"

DEFINE_GAMEPLAY_TAG(GTag_Ability_Action_Weapon, "Ability.Action.Weapon", TEXT("This ability changes weapon state"));
DEFINE_GAMEPLAY_TAG(GTag_Unit_State_Weapon_Sheathed, "Unit.State.Weapon.Sheathed", TEXT("Weapon is sheathed"));
DEFINE_GAMEPLAY_TAG(GTag_Unit_State_Weapon_Drawing, "Unit.State.Weapon.Drawing", TEXT("Weapon is being drawn out"));
DEFINE_GAMEPLAY_TAG(GTag_Unit_State_Weapon_Drawn, "Unit.State.Weapon.Drawn", TEXT("Weapon is drawn out"));
DEFINE_GAMEPLAY_TAG(GTag_Unit_State_Weapon_Sheathing, "Unit.State.Weapon.Sheathing", TEXT("Weapon is being sheathed"));

UGameplayAbility_WeaponToggle::UGameplayAbility_WeaponToggle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Important! This must NOT be set to NonInstanced!
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// All child classes have this tag by default
	AbilityTags.AddTag(GTag_Ability_Action_Weapon);

	// Block other abilities of same type (i.e., change the weapon state)
	BlockAbilitiesWithTag.AddTag(GTag_Ability_Action_Weapon);
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

	const UEquipmentManagerComponent* EquipmentManager = Character->GetEquipmentManagerComponent();

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
	AbilitySystemComponent->SetLooseGameplayTagCount(GTag_Unit_State_Weapon_Drawn, 1);
}

void UGameplayAbility_WeaponToggle::RemoveWeaponDrawnTag()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Checked();
	AbilitySystemComponent->SetLooseGameplayTagCount(GTag_Unit_State_Weapon_Drawn, 0);
}

bool UGameplayAbility_WeaponToggle::GetDrawMontage(UAnimMontage*& DrawMontage) const
{
	const UEquipmentManagerComponent* EquipmentManager = GetEquipmentManagerFromActorInfo();
	check(EquipmentManager); // Guaranteed by CanActivateAbility override
	const UItemInstance* CurrentWeapon = EquipmentManager->GetCurrentWeapon();
	check(CurrentWeapon); // Guaranteed by CanActivateAbility override
	const UItemProperty_Weapon* WeaponProperty = CurrentWeapon->GetProperty<UItemProperty_Weapon>();
	check(WeaponProperty); // Guaranteed by UEquipmentManagerComponent::EquipWeapon

	DrawMontage = WeaponProperty->GetDrawMontage();
	return DrawMontage != nullptr;
}

bool UGameplayAbility_WeaponToggle::GetSheatheMontage(UAnimMontage*& SheatheMontage) const
{
	const UEquipmentManagerComponent* EquipmentManager = GetEquipmentManagerFromActorInfo();
	check(EquipmentManager); // Guaranteed by CanActivateAbility override
	const UItemInstance* CurrentWeapon = EquipmentManager->GetCurrentWeapon();
	check(CurrentWeapon); // Guaranteed by CanActivateAbility override
	const UItemProperty_Weapon* WeaponProperty = CurrentWeapon->GetProperty<UItemProperty_Weapon>();
	check(WeaponProperty); // Guaranteed by UEquipmentManagerComponent::EquipWeapon

	SheatheMontage = WeaponProperty->GetSheatheMontage();
	return SheatheMontage != nullptr;
}
