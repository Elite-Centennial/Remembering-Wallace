// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSetBase.h"

#include "HealthAttributeSet.generated.h"

/**
 * Attribute set for units that are capable for getting damaged
 */
UCLASS()
class REMEMBERINGWALLACE_API UHealthAttributeSet : public UAttributeSetBase
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UHealthAttributeSet();

	// Static methods to return the attribute itself
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UHealthAttributeSet, Health);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UHealthAttributeSet, MaxHealth);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UHealthAttributeSet, Defense);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UHealthAttributeSet, Damage);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UHealthAttributeSet, Heal);

	// Getters for the attribute values
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Health);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxHealth);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Defense);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Damage);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Heal);

	// BEGIN UAttributeSet interface
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostAttributeBaseChange(
		const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;
	// END UAttributeSet interface

protected:
	// Setters for the base values of attributes
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Health);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MaxHealth);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Defense);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Damage);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Heal);

	/**
	 * Current health
	 *
	 * This attribute is hidden from modifiers to prevent any unintended modifications. To make health go down, add to
	 * Damage attribute instead. To do the opposite, use Heal attribute.
	 *
	 * Only the base value is relevant. It doesn't make sense to have a temporary buff that increases the current health
	 * by some amount and revert it back when it expires. Preventing direct modifications already prevents this, but it
	 * is worth noting that we only need to care about the base value for this attribute.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health", meta = (HideFromModifiers))
	FGameplayAttributeData Health;

	/**
	 * Maximum value for the current health
	 *
	 * This attribute can have temporary effects that change the final computed value without changing the actual base
	 * value. That means whenever a buff on this attribute expires, Health might be larger than the new value of this
	 * attribute. Therefore, we want to detect all changes to this attribute and clamp the value for Health.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health")
	FGameplayAttributeData MaxHealth;

	/**
	 * How much of the incoming damage is mitigated
	 *
	 * Both the base value and effective value should be non-negative.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health")
	FGameplayAttributeData Defense;

	/**
	 * Incoming damage, directly mapped to -Health
	 *
	 * This is a meta-attribute which is stateless and is reset on every update on this attribute set.
	 *
	 * All changes to the base value are incoming damage. Negative values are clamped. There could be gameplay effects
	 * that provide temporary modifications to adjust damage mitigation (mainly according to Defense attribute), but we
	 * are only interested in when the actual incoming damage takes place. Therefore, we process this meta-attribute and
	 * apply the damage to Health only on changes to the base value. We also listen for changes in the current value
	 * only for clamping.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health")
	FGameplayAttributeData Damage;

	/**
	 * Incoming healing, directly mapped to +Health
	 *
	 * This is a meta-attribute which is stateless and is reset on every update on this attribute set.
	 *
	 * All changes to the base value are incoming healing. Negative values are clamped. There could be gameplay effects
	 * that provide temporary modifications to adjust healing received, but we are only interested in when the actual
	 * incoming healing takes place. Therefore, we process this meta-attribute and apply the healing to Health only on
	 * changes to the base value. We also listen for changes in the current value only for clamping.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health")
	FGameplayAttributeData Heal;

	/**
	 * Set the value for bOutOfHealth flag
	 *
	 * This version checks the current value of Health attribute.
	 */
	void SetOutOfHealth() { bOutOfHealth = GetHealth() <= 0.0f; }

	/**
	 * Set the value for bOutOfHealth flag
	 *
	 * This version overrides the value with the provided one.
	 */
	void SetOutOfHealth(const bool bInOutOfHealth) { bOutOfHealth = bInOutOfHealth; }

private:
	/**
	 * A flag to keep track of whether the owner is out of health
	 */
	bool bOutOfHealth;

	/**
	 * Enforce the value ranges for the new incoming value
	 */
	void ClampNewAttributeValue(const FGameplayAttribute& Attribute, float& NewValue) const;
};
