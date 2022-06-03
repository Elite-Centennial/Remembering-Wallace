// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSetBase.h"

#include "CombatAttributeSet.generated.h"

/**
 * Attribute set for units that are capable of doing damage to other units
 */
UCLASS()
class REMEMBERINGWALLACE_API UCombatAttributeSet : public UAttributeSetBase
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UCombatAttributeSet();

	// Static methods to return the attribute itself
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UCombatAttributeSet, Mana);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UCombatAttributeSet, MaxMana);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UCombatAttributeSet, Attack);

	// Getters for the attribute values
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Mana);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxMana);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Attack);

	// BEGIN UAttributeSet interface
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	// END UAttributeSet interface

protected:
	// Setters for the attribute values
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Mana);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MaxMana);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Attack);

	/**
	 * Current mana
	 *
	 * Although this is not hidden from modifiers, this attribute must only be changed by instant or periodic gameplay
	 * effects. It doesn't make sense to have a duration-based buff or debuff on this attribute. Only the base value is
	 * relevant.
	 *
	 * There is little need for meta-attributes for this one because the required control on the amount of incoming
	 * changes is not as strict or critical as the one for Health.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Combat")
	FGameplayAttributeData Mana;

	/**
	 * Maximum value for current mana
	 *
	 * This attribute can have temporary effects that change the final computed value without changing the actual base
	 * value. That means whenever a buff on this attribute expires, Mana might be larger than the new value of this
	 * attribute. Therefore, we want to detect all changes to this attribute and clamp the value for Mana.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Combat")
	FGameplayAttributeData MaxMana;

	/**
	 * How big of a damage the owner can do to other units
	 *
	 * Both the base value and effective value should be 1.0 or greater.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Combat")
	FGameplayAttributeData Attack;

private:
	/**
	 * A flag to keep track of whether the owner is out of mana
	 */
	bool bOutOfMana;

	/**
	 * Enforce the value ranges for the new incoming value
	 */
	void ClampNewAttributeValue(const FGameplayAttribute& Attribute, float& NewValue) const;
};
