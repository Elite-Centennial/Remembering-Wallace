// Elite Centennial

#pragma once

#include "RememberingWallaceTags.h"

/**
 * Tag for event when the unit takes a damage
 */
REMEMBERINGWALLACE_API DECLARE_GAMEPLAY_TAG(GTag_Event_Combat_Damaged);

/**
 * Tag for event when the unit does a damage
 */
REMEMBERINGWALLACE_API DECLARE_GAMEPLAY_TAG(GTag_Event_Combat_DidDamage);

/**
 * Tag for event when the unit receives a heal
 */
REMEMBERINGWALLACE_API DECLARE_GAMEPLAY_TAG(GTag_Event_Combat_Healed);

/**
 * Tag for event when the unit does a heal
 */
REMEMBERINGWALLACE_API DECLARE_GAMEPLAY_TAG(GTag_Event_Combat_DidHeal);

/**
 * Tag for event when the unit's health goes down to zero
 */
REMEMBERINGWALLACE_API DECLARE_GAMEPLAY_TAG(GTag_Event_Unit_OutOfHealth);

/**
 * Tag for event when the unit's mana goes down to zero
 */
REMEMBERINGWALLACE_API DECLARE_GAMEPLAY_TAG(GTag_Event_Unit_OutOfMana);
