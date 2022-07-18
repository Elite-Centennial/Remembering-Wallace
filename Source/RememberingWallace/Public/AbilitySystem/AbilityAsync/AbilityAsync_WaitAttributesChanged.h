// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Async/AbilityAsync.h"

#include "AbilityAsync_WaitAttributesChanged.generated.h"

/**
 * A BlueprintAsyncAction that listens for changes in attribute values
 *
 * This version adapts the code in UAbilityAsync_WaitAttributeChanged provided by the Gameplay Abilities plugin. Mainly,
 * there are 3 differences:
 * 1. This version enables the user to listen for changes in multiple attributes at once. While this is still achievable
 *    with the original version, the user has to create a new node for every attribute they're interested in.
 * 2. This version provides an option to configure if the delegate should be broadcast even when there was no change in
 *    the value. The original version does not check the values and sends the event regardless of whether the value
 *    actually changed or not.
 * 3. This version provides an option to broadcast the delegate as soon as it is bound. This allows the user to get the
 *    initial value without building the extra logic themself. In this case, OldValue is set to -1.0.
 */
UCLASS()
class REMEMBERINGWALLACE_API UAbilityAsync_WaitAttributesChanged : public UAbilityAsync
{
	GENERATED_BODY()

public:
	/**
	 * A BlueprintAsyncAction that listens for changes in attribute values
	 *
	 * This version adapts the code in UAbilityAsync_WaitAttributeChanged provided by the Gameplay Abilities plugin.
	 * Mainly, there are 3 differences:
	 * 1. This version enables the user to listen for changes in multiple attributes at once. While this is still
	 *    achievable with the original version, the user has to create a new node for every attribute they're interested
	 *    in.
	 * 2. This version provides an option to configure if the delegate should be broadcast even when there was no change
	 *    in the value. The original version does not check the values and sends the event regardless of whether the
	 *    value actually changed or not.
	 * 3. This version provides an option to broadcast the delegate as soon as it is bound. This allows the user to get
	 *    the initial value without building the extra logic themself. In this case, OldValue is set to -1.0.
	 *
	 * TargetActor is the actor that implements IAbilitySystemComponent or has UAbilitySystemComponent attached to it.
	 * To explicitly specify ASC, use WaitForAttributesChangedOnAbilitySystemComponent instead.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "true"))
	static UAbilityAsync_WaitAttributesChanged* WaitForAttributesChangedOnActor(
		AActor* TargetActor,
		const TArray<FGameplayAttribute>& Attributes,
		bool bOnlyTriggerOnce = false,
		bool bBroadcastInitialValue = true,
		bool bIgnoreNoValueChange = true);

	/**
	 * A BlueprintAsyncAction that listens for changes in attribute values
	 *
	 * This version adapts the code in UAbilityAsync_WaitAttributeChanged provided by the Gameplay Abilities plugin.
	 * Mainly, there are 3 differences:
	 * 1. This version enables the user to listen for changes in multiple attributes at once. While this is still
	 *    achievable with the original version, the user has to create a new node for every attribute they're interested
	 *    in.
	 * 2. This version provides an option to configure if the delegate should be broadcast even when there was no change
	 *    in the value. The original version does not check the values and sends the event regardless of whether the
	 *    value actually changed or not.
	 * 3. This version provides an option to broadcast the delegate as soon as it is bound. This allows the user to get
	 *    the initial value without building the extra logic themself. In this case, OldValue is set to -1.0.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (BlueprintInternalUseOnly = "true"))
	static UAbilityAsync_WaitAttributesChanged* WaitForAttributesChangedOnAbilitySystemComponent(
		UAbilitySystemComponent* TargetAbilitySystemComponent,
		const TArray<FGameplayAttribute>& Attributes,
		bool bOnlyTriggerOnce = false,
		bool bBroadcastInitialValue = true,
		bool bIgnoreNoValueChange = true);

	/**
	 * Type of delegate that is broadcast when an attribute changes
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
		FAbilityAsyncWaitAttributesChangedDelegate,
		const FGameplayAttribute&, Attribute,
		float, NewValue,
		float, OldValue);

	/**
	 * The delegate that is broadcast when an attribute changes
	 */
	UPROPERTY(BlueprintAssignable)
	FAbilityAsyncWaitAttributesChangedDelegate Changed;

	// BEGIN UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	// END UBlueprintAsyncActionBase interface

	// BEGIN UAbilityAsync interface
	virtual void EndAction() override;
	// END UAbilityAsync interface

protected:
	/**
	 * Callback to register to ASC
	 */
	void OnAttributeChanged(const FOnAttributeChangeData& ChangeData);

	/**
	 * Whether this task should terminate itself after being invoked once
	 */
	bool bOnlyTriggerOnce = false;

	/**
	 * Whether this task should broadcast the event as soon as the task starts
	 *
	 * The initial broadcast does not mean that there was a change. This option is just provided as a utility to receive
	 * information about the initial state. The value for OldValue is set to -1.0 for this initial broadcast.
	 */
	bool bBroadcastInitialValue = true;

	/**
	 * Whether this task should ignore events with equal values of NewValue and OldValue
	 *
	 * Enable this option if you only need to track the value.
	 */
	bool bIgnoreNoValueChange = true;

	/**
	 * Simple data struct to store the delegate handles for each attribute
	 */
	struct FAttributeDelegateHandle
	{
		FAttributeDelegateHandle(const FGameplayAttribute& InAttribute) : Attribute(InAttribute)
		{
		}

		/**
		 * The attribute this task listens for changes
		 */
		FGameplayAttribute Attribute;

		/**
		 * Saved handle for the delegate registered to the ASC
		 */
		FDelegateHandle DelegateHandle;
	};

	/**
	 * List of attributes along with delegate handles
	 */
	TArray<FAttributeDelegateHandle> Attributes;
};
