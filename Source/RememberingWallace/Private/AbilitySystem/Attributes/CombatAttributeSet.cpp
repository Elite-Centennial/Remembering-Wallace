// Elite Centennial

#include "AbilitySystem/Attributes/CombatAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "AbilitySystem/GameplayEventHelper.h"
#include "AbilitySystem/GameplayEventTags.h"

UCombatAttributeSet::UCombatAttributeSet()
	: Mana(50.0f),
	  MaxMana(50.0f),
	  Attack(10.0f),
	  bOutOfMana(false)
{
}

// This is called just before the gameplay effect is actually executed. The change to the attribute is already made. We
// cannot make any changes to the passed-in data. This is where we send OutOfMana event.
void UCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		// Exit early if the owner is not out of mana
		if (GetMana() > 0.0f)
		{
			// Make sure the flag is set to the right value
			bOutOfMana = false;
			return;
		}

		if (!bOutOfMana)
		{
			// The owner was not out of mana before, but now it is
			// Send OutOfMana event
			FGameplayEventData EventData;
			RememberingWallace::GameplayEvent::GatherEventDataForAttrGEExecute(
				&Data, Data.EvaluatedData.Magnitude, &EventData);
			EventData.EventTag = GTag_Event_Unit_OutOfMana;
			GetOwningAbilitySystemComponentChecked()->HandleGameplayEvent(EventData.EventTag, &EventData);
		}

		// Update the flag with the current value in case it changed due to the above event
		bOutOfMana = GetMana() <= 0.0f;
	}
}

// This is called whenever the current value of an attribute is about to change for whatever reason. We can enforce
// limits and clamp the new value. For simplicity, we only handle the attribute that is being changed. If changes to
// other attributes are needed, use `PostAttributeChange`.
void UCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampNewAttributeValue(Attribute, NewValue);
}

// This is called after a change in the current value of the attribute is applied. We can adjust values of other
// attributes according to the change.
void UCombatAttributeSet::PostAttributeChange(
	const FGameplayAttribute& Attribute, const float OldValue, const float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxManaAttribute())
	{
		// Current mana might be greater than max mana, so we need to clamp the value if that's the case
		if (GetMana() > NewValue)
		{
			SetMana(NewValue);
		}
	}
}

// This is called when the base value changes. We're only allowed to make changes to the soon-to-be value. The value
// we're clamping here is the base value.
void UCombatAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampNewAttributeValue(Attribute, NewValue);
}

void UCombatAttributeSet::ClampNewAttributeValue(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetManaAttribute())
	{
		// Mana should be between 0.0 and max mana
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		// Max mana should never go below 1.0
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetAttackAttribute())
	{
		// Attack should never be negative
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}
