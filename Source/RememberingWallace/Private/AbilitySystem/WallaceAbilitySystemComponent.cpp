// Elite Centennial

#include "AbilitySystem/WallaceAbilitySystemComponent.h"

#include "AbilitySystem/AbilityInputID.h"
#include "AbilitySystem/Abilities/GameplayAbilityBase.h"

UWallaceAbilitySystemComponent::UWallaceAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWallaceAbilitySystemComponent::BindToInputComponent(UInputComponent* InputComponent)
{
	// This parent class implementation binds "AbilityConfirm" and "AbilityCancel" input actions to the generic target
	// confirm/cancel functionalities.
	Super::BindToInputComponent(InputComponent);

	// Or, if we want to use Attack/Block input IDs for confirm/cancel actions and have the input only do the confirm/
	// cancel when there's an ability listening for target data, comment out the part above and instead uncomment the
	// following lines:
	// GenericConfirmInputID = static_cast<uint8>(EAbilityInputID::Attack);
	// GenericCancelInputID = static_cast<uint8>(EAbilityInputID::Block)

	// Get the enum for input IDs
	const UEnum* InputIDs = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EAbilityInputID"));

	// Match the length of BlockedAbilityBindings array to the number of input IDs
	GetBlockedAbilityBindings_Mutable().SetNumZeroed(InputIDs->NumEnums());

	// Bind to the input components according to the input IDs enum
	// We skip index 0 because value None is NOT meant to be bound to player input.
	for (int32 Idx = 1; Idx < InputIDs->NumEnums(); ++Idx)
	{
		const FString FullStr = InputIDs->GetNameStringByIndex(Idx);

		// Pressed event
		FInputActionBinding PressedBinding(FName(*FullStr), IE_Pressed);
		PressedBinding
			.ActionDelegate
			.GetDelegateForManualSet()
			.BindUObject(this, &UAbilitySystemComponent::AbilityLocalInputPressed, Idx);
		InputComponent->AddActionBinding(PressedBinding);

		// Released event
		FInputActionBinding ReleasedBinding(FName(*FullStr), IE_Released);
		ReleasedBinding
			.ActionDelegate
			.GetDelegateForManualSet()
			.BindUObject(this, &UAbilitySystemComponent::AbilityLocalInputReleased, Idx);
		InputComponent->AddActionBinding(ReleasedBinding);
	}
}

void UWallaceAbilitySystemComponent::AbilityLocalInputPressed(const int32 InputID)
{
	// This override is almost the same as the parent implementation except that
	// 1. Replication-related things are removed
	// 2. bActivateOnInput is checked on each ability before being activated from the input

	// Consume the input if this InputID is GenericConfirm and something is listening to the confirm
	if (IsGenericConfirmInputBound(InputID))
	{
		LocalInputConfirm();
		return;
	}

	// Consume the input if this InputID is GenericCancel and something is listening to the cancel
	if (IsGenericCancelInputBound(InputID))
	{
		LocalInputCancel();
		return;
	}

	// This is needed to prevent the array of activatable abilities from being modified while we're going through the
	// for loop.
	ABILITYLIST_SCOPE_LOCK();

	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.InputID != InputID || !Spec.Ability)
		{
			continue;
		}

		Spec.InputPressed = true;

		// If already active, just send the input pressed event
		if (Spec.IsActive())
		{
			AbilitySpecInputPressed(Spec);
			InvokeReplicatedEvent(
				EAbilityGenericReplicatedEvent::InputPressed,
				Spec.Handle,
				Spec.ActivationInfo.GetActivationPredictionKey());
			continue;
		}

		// If the ability is of our custom type, check bActivateOnInput and skip if false
		if (const UGameplayAbilityBase* Ability = Cast<UGameplayAbilityBase>(Spec.Ability))
		{
			if (!Ability->ShouldActivateOnInput())
			{
				continue;
			}
		}

		// All checks passed; activate the ability
		TryActivateAbility(Spec.Handle);
	}
}

void UWallaceAbilitySystemComponent::AbilityLocalInputReleased(const int32 InputID)
{
	// This override is almost the same as the parent implementation except that
	// 1. Replication-related things are removed

	// This is needed to prevent the array of activatable abilities from being modified while we're going through the
	// for loop.
	ABILITYLIST_SCOPE_LOCK();

	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.InputID != InputID)
		{
			continue;
		}

		Spec.InputPressed = false;

		if (Spec.Ability && Spec.IsActive())
		{
			AbilitySpecInputReleased(Spec);
			InvokeReplicatedEvent(
				EAbilityGenericReplicatedEvent::InputReleased,
				Spec.Handle,
				Spec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}
