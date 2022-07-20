// Elite Centennial

#include "AbilitySystem/Attributes/HealthAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "RememberingWallaceTags.h"
#include "AbilitySystem/GameplayEventHelper.h"

UHealthAttributeSet::UHealthAttributeSet()
	: Health(100.0f),
	  MaxHealth(100.0f),
	  Defense(0.0f),
	  Damage(0.0f),
	  Heal(0.0f),
	  bOutOfHealth(false)
{
}

// This is called when a gameplay effect is about to make a change to the base value of an attribute. We can refuse the
// change by returning `false`. We can also modify the passed-in data including what the new value will be.
bool UHealthAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	// Put the code here if implementing damage immunity / disabling healing / etc.

	return true;
}

// This is called just before the gameplay effect is actually executed. The change to the attribute is already made. We
// cannot make any changes to the passed-in data. This is where we convert the damage and heal attributes to the actual
// health attribute.
void UHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Retrieve the damage value and reset the attribute
		const float IncomingDamage = GetDamage();
		SetDamage(0.0f);

		if (IncomingDamage <= 0.0f)
		{
			// Only handle positive values
			return;
		}

		// First, we actually apply the damage. Next, we send gameplay events related to doing and receiving damage.
		// Lastly, we check if the health dropped to zero and if so, we send OutOfHealth gameplay event.

		// Apply to health
		// Note(Dohyun): If we need to send data about the health before and after, we need to create our own class of
		// gameplay effect context and add additional data to it.
		SetHealth(GetHealth() - IncomingDamage);

		// Send DidDamage and Damaged gameplay events
		// Keep the event data as we might need to send OutOfHealth event
		FGameplayEventData EventData;
		RememberingWallace::GameplayEvent::SendEventsForAttrGEExecute(
			&Data,
			GetOwningAbilitySystemComponentChecked(),
			TAG(Event.Combat.DidDamage),
			TAG(Event.Combat.Damaged),
			IncomingDamage,
			&EventData);

		// Exit early if the new health is above zero
		if (GetHealth() > 0.0f)
		{
			// Let's just make sure the flag is set correctly
			SetOutOfHealth(false);
			return;
		}

		if (!bOutOfHealth)
		{
			// The owner was not out of health before, but now it is
			// Send OutOfHealth event
			EventData.EventTag = TAG(Event.Unit.OutOfHealth);
			GetOwningAbilitySystemComponentChecked()->HandleGameplayEvent(EventData.EventTag, &EventData);
		}

		// Update the flag with the current health value in case the value changed due to the event
		SetOutOfHealth();
	}
	else if (Data.EvaluatedData.Attribute == GetHealAttribute())
	{
		// Retrieve the heal value and reset the attribute
		const float IncomingHeal = GetHeal();
		SetHeal(0.0f);

		if (IncomingHeal <= 0.0f)
		{
			// Only handle positive values
			return;
		}

		// First, we actually apply the heal. Next, we send gameplay events related to doing and receiving a heal.

		// Apply to health
		// Note(Dohyun): If we need to send data about the health before and after, we need to create our own class of
		// gameplay effect context and add additional data to it.
		SetHealth(GetHealth() + IncomingHeal);

		// Send DidHeal and Healed gameplay events
		RememberingWallace::GameplayEvent::SendEventsForAttrGEExecute(
			&Data,
			GetOwningAbilitySystemComponentChecked(),
			TAG(Event.Combat.DidHeal),
			TAG(Event.Combat.Healed),
			IncomingHeal);

		// Update the flag with the current value in case the value changed due to the event
		SetOutOfHealth();
	}
}

// This is called whenever the current value of an attribute is about to change for whatever reason. We can enforce
// limits and clamp the new value. For simplicity, we only handle the attribute that is being changed. If changes to
// other attributes are needed, use `PostAttributeChange`.
void UHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampNewAttributeValue(Attribute, NewValue);
}

// This is called after a change in the current value of the attribute is applied. We can adjust values of other
// attributes according to the change.
void UHealthAttributeSet::PostAttributeChange(
	const FGameplayAttribute& Attribute, const float OldValue, const float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		// Current health might be larger than max health, so we need to clamp the value if that's the case
		if (GetHealth() > NewValue)
		{
			SetHealth(NewValue);
		}
	}
}

// This is called when the base value changes. We're only allowed to make changes to the soon-to-be value. The value
// we're clamping here is the base value.
void UHealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampNewAttributeValue(Attribute, NewValue);
}

// This is called after the change to the base value is applied. This is a const function, so we can't really make any
// changes.
void UHealthAttributeSet::PostAttributeBaseChange(
	const FGameplayAttribute& Attribute, const float OldValue, const float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UHealthAttributeSet::ClampNewAttributeValue(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		// Health should be between 0.0 and max health
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// Max health should never go below 1.0
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetDefenseAttribute())
	{
		// Defense should never be negative
		NewValue = FMath::Max(NewValue, 0.0f);
	}
	else if (Attribute == GetDamageAttribute() || Attribute == GetHealAttribute())
	{
		// Damage and heal should never be negative
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}
