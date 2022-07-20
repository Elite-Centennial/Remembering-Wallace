// Elite Centennial

#include "RememberingWallaceTags.h"

#include "NativeGameplayTags.h"

// Define a static native gameplay tag inside the constructor and assign it to the field
#define CONSTRUCT_TAG(TagName, Tag, DevComment) \
	static const FNativeGameplayTag Tag_##TagName( \
		UE_PLUGIN_NAME, UE_MODULE_NAME, Tag, TEXT(DevComment), ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD); \
	TagName = Tag_##TagName.GetTag();

FWallaceTags::FAbility::FAbility()
{
	CONSTRUCT_TAG(Internal_RootTag, "Ability", "Gameplay abilities");
}

FWallaceTags::FAbility::FAction::FAction()
{
	CONSTRUCT_TAG(Internal_RootTag, "Ability.Action", "Abilities that fire off one-time actions");
	CONSTRUCT_TAG(Weapon, "Ability.Action.Weapon", "This ability changes weapon state");
}

FWallaceTags::FEvent::FEvent()
{
	CONSTRUCT_TAG(Internal_RootTag, "Event", "Gameplay events sent to ASCs");
}

FWallaceTags::FEvent::FCombat::FCombat()
{
	CONSTRUCT_TAG(Internal_RootTag, "Event.Combat", "Gameplay events related to combat");
	CONSTRUCT_TAG(Damaged, "Event.Combat.Damaged", "Unit took a damage");
	CONSTRUCT_TAG(DidDamage, "Event.Combat.DidDamage", "Unit applied a damage");
	CONSTRUCT_TAG(Healed, "Event.Combat.Healed", "Unit received a heal");
	CONSTRUCT_TAG(DidHeal, "Event.Combat.DidHeal", "Unit applied a heal");
}

FWallaceTags::FEvent::FUnit::FUnit()
{
	CONSTRUCT_TAG(Internal_RootTag, "Event.Unit", "Gameplay events related to combat units");
	CONSTRUCT_TAG(OutOfHealth, "Event.Unit.OutOfHealth", "Health dropped to zero");
	CONSTRUCT_TAG(OutOfMana, "Event.Unit.OutOfMana", "Mana dropped to zero");
}

FWallaceTags::FUnit::FUnit()
{
	CONSTRUCT_TAG(Internal_RootTag, "Unit", "Combat units' states, traits, etc.");
}

FWallaceTags::FUnit::FState::FState()
{
	CONSTRUCT_TAG(Internal_RootTag, "Unit.State", "States of combat units");
	CONSTRUCT_TAG(Immobilized, "Unit.State.Immobilized", "Movement is prohibited");
}

FWallaceTags::FWeapon::FWeapon()
{
	CONSTRUCT_TAG(Internal_RootTag, "Weapon", "Tags related to weapons");
}

FWallaceTags::FWeapon::FState::FState()
{
	CONSTRUCT_TAG(Internal_RootTag, "Weapon.State", "Weapon states like sheathed or drawn");
	CONSTRUCT_TAG(Drawing, "Weapon.State.Drawing", "Weapon is being drawn out");
	CONSTRUCT_TAG(Drawn, "Weapon.State.Drawn", "Weapon is drawn out");
	CONSTRUCT_TAG(Sheathing, "Weapon.State.Sheathing", "Weapon is being sheathed");
	CONSTRUCT_TAG(Sheathed, "Weapon.State.Sheathed", "Weapon is sheathed");
}

const FGameplayTagContainer& FWallaceTags::FWeapon::FState::GetAll() const
{
	static const FGameplayTagContainer WeaponStateTags = FGameplayTagContainer::CreateFromArray(
		TArray{
			Drawing,
			Drawn,
			Sheathing,
			Sheathed,
		});
	return WeaponStateTags;
}

FWallaceTags::FWeapon::FType::FType()
{
	CONSTRUCT_TAG(Internal_RootTag, "Weapon.Type", "Types of weapons");
}

const FWallaceTags FWallaceTags::Tags;
