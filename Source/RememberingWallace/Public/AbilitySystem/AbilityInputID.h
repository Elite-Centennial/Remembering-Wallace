// Elite Centennial

#pragma once

#include "CoreMinimal.h"

#include "AbilityInputID.generated.h"

/**
 * List of input IDs to be used to bind gameplay abilities to player input
 */
UENUM(BlueprintType, Category = "AbilitySystem")
enum class EAbilityInputID : uint8
{
	// Not bound to anything
	None,
	// Jump action
	Jump,
	// Dodge action
	Dodge,
	// Sprint
	Sprint,
	// Draw / sheathe weapon
	ToggleWeaponState,
	// Default attack
	Attack,
	// Block
	Block,
	// Ability slot 1
	Ability1,
	// Ability slot 2
	Ability2,
	// Ability slot 3
	Ability3,
};
