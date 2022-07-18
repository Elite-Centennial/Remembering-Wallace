// Elite Centennial

#include "AbilitySystem/AbilityAsync/AbilityAsync_WaitAttributesChanged.h"

#include "AbilitySystemComponent.h"

UAbilityAsync_WaitAttributesChanged* UAbilityAsync_WaitAttributesChanged::WaitForAttributesChangedOnActor(
	AActor* TargetActor,
	const TArray<FGameplayAttribute>& Attributes,
	const bool bOnlyTriggerOnce,
	const bool bBroadcastInitialValue,
	const bool bIgnoreNoValueChange)
{
	UAbilityAsync_WaitAttributesChanged* Async = NewObject<UAbilityAsync_WaitAttributesChanged>();

	Async->SetAbilityActor(TargetActor);
	Async->Attributes.Reserve(Attributes.Num());

	for (const FGameplayAttribute& Attribute : Attributes)
	{
		Async->Attributes.Emplace(Attribute);
	}

	Async->bOnlyTriggerOnce = bOnlyTriggerOnce;
	Async->bBroadcastInitialValue = bBroadcastInitialValue;
	Async->bIgnoreNoValueChange = bIgnoreNoValueChange;

	return Async;
}

UAbilityAsync_WaitAttributesChanged*
UAbilityAsync_WaitAttributesChanged::WaitForAttributesChangedOnAbilitySystemComponent(
	UAbilitySystemComponent* TargetAbilitySystemComponent,
	const TArray<FGameplayAttribute>& Attributes,
	const bool bOnlyTriggerOnce,
	const bool bBroadcastInitialValue,
	const bool bIgnoreNoValueChange)
{
	UAbilityAsync_WaitAttributesChanged* Async = NewObject<UAbilityAsync_WaitAttributesChanged>();

	Async->SetAbilitySystemComponent(TargetAbilitySystemComponent);
	Async->Attributes.Reserve(Attributes.Num());

	for (const FGameplayAttribute& Attribute : Attributes)
	{
		Async->Attributes.Emplace(Attribute);
	}

	Async->bOnlyTriggerOnce = bOnlyTriggerOnce;
	Async->bBroadcastInitialValue = bBroadcastInitialValue;
	Async->bIgnoreNoValueChange = bIgnoreNoValueChange;

	return Async;
}

void UAbilityAsync_WaitAttributesChanged::Activate()
{
	Super::Activate();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		// ASC is valid; bind delegates
		for (FAttributeDelegateHandle& Attribute : Attributes)
		{
			Attribute.DelegateHandle =
				ASC->GetGameplayAttributeValueChangeDelegate(Attribute.Attribute)
					.AddUObject(this, &ThisClass::OnAttributeChanged);

			if (bBroadcastInitialValue)
			{
				// Broadcast the delegate with the initial value if requested
				const float CurrentValue = ASC->GetNumericAttribute(Attribute.Attribute);
				Changed.Broadcast(Attribute.Attribute, CurrentValue, -1.0f);
			}
		}
	}
	else
	{
		// ASC is invalid; end this task
		EndAction();
	}
}

void UAbilityAsync_WaitAttributesChanged::EndAction()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		// Unbind delegates
		for (const auto& [Attribute, Handle] : Attributes)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Attribute).Remove(Handle);
		}

		// Clear the array to free memory
		Attributes.Empty();
	}

	Super::EndAction();
}

void UAbilityAsync_WaitAttributesChanged::OnAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	if (!ShouldBroadcastDelegates())
	{
		// ASC not valid anymore
		EndAction();
		return;
	}

	if (bIgnoreNoValueChange && ChangeData.NewValue == ChangeData.OldValue)
	{
		// Ignore if there was no change in the value
		return;
	}

	// Broadcast the event
	Changed.Broadcast(ChangeData.Attribute, ChangeData.NewValue, ChangeData.OldValue);

	if (bOnlyTriggerOnce)
	{
		// Clean up if only need to get the event once
		EndAction();
	}
}
