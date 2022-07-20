// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Get the gameplay tag for the specified path, defined in FWallaceTags
 */
#define TAG(TagPath) (FWallaceTags::Get().TagPath)

// Internal struct definitions for the tags structure
namespace RememberingWallace::Tags
{
	// Base class for a collection of gameplay tags which namespace also corresponds to an explicit tag
	struct FTagsWithRootTag
	{
		operator FGameplayTag() const { return Internal_RootTag; }
		operator const FGameplayTag&() const { return Internal_RootTag; }
		const FGameplayTag& GetTag() const { return Internal_RootTag; }

	protected:
		FGameplayTag Internal_RootTag;
	};
}

/**
 * A collection of all gameplay tags used in the native code
 *
 * Use the utility macro TAG this like: const FGameplayTag& WeaponDrawnTag = TAG(Weapon.State.Drawn);
 */
struct FWallaceTags
{
	/**
	 * Return the singleton instance of the collection of gameplay tags
	 */
	static const FWallaceTags& Get() { return Tags; }

	// "Ability": Tags for gameplay abilities
	struct FAbility : RememberingWallace::Tags::FTagsWithRootTag
	{
		FAbility();

		// "Ability.Action": Abilities that fire off one-time actions
		struct FAction : FTagsWithRootTag
		{
			FAction();
			FGameplayTag Weapon; // "Ability.Action.Weapon": Abilities that switch weapon state
		} Action;
	} Ability;

	// "Event": Tags for gameplay events
	struct FEvent : RememberingWallace::Tags::FTagsWithRootTag
	{
		FEvent();

		// "Event.Combat": Gameplay events related to combat
		struct FCombat : FTagsWithRootTag
		{
			FCombat();
			FGameplayTag Damaged; // "Event.Combat.Damaged": Took a damage
			FGameplayTag DidDamage; // "Event.Combat.DidDamage": Applied a damage
			FGameplayTag Healed; // "Event.Combat.Healed": Got a heal
			FGameplayTag DidHeal; // "Event.Combat.DidHeal": Applied a heal
		} Combat;

		// "Event.Unit": Gameplay events where something happened to a combat unit
		struct FUnit : FTagsWithRootTag
		{
			FUnit();
			FGameplayTag OutOfHealth; // "Event.Unit.OutOfHealth": Health dropped to zero
			FGameplayTag OutOfMana; // "Event.Unit.OutOfMana": Mana dropped to zero
		} Unit;
	} Event;

	// "Unit": Tags related to combat units
	struct FUnit : RememberingWallace::Tags::FTagsWithRootTag
	{
		FUnit();

		// "Unit.State": States for units
		struct FState : FTagsWithRootTag
		{
			FState();
			FGameplayTag Immobilized; // "Unit.State.Immobilized": Movement is not allowed
		} State;
	} Unit;

	// "Weapon": Tags for weapons
	struct FWeapon : RememberingWallace::Tags::FTagsWithRootTag
	{
		FWeapon();

		// "Weapon.State": Weapon states
		struct FState : FTagsWithRootTag
		{
			FState();
			FGameplayTag Drawing; // "Weapon.State.Drawing"
			FGameplayTag Drawn; // "Weapon.State.Drawn"
			FGameplayTag Sheathing; // "Weapon.State.Sheathing"
			FGameplayTag Sheathed; // "Weapon.State.Sheathed"
			const FGameplayTagContainer& GetAll() const; // All weapon state tags
		} State;

		// "Weapon.Type": Weapon types assigned to weapon item property
		struct FType : FTagsWithRootTag
		{
			FType();
		} Type;
	} Weapon;

private:
	static const FWallaceTags Tags; // Singleton instance
};
