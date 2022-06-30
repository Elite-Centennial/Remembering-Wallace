// Elite Centennial

#include "Animation/CharacterAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Character/CharacterBase.h"
#include "Character/CharacterBaseMovementComponent.h"

UCharacterAnimInstance::UCharacterAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const ACharacterBase* UCharacterAnimInstance::GetCharacter() const
{
	// NOTE(Dohyun): Do not use CastChecked as the editor preview might use its own actor class
	return Cast<ACharacterBase>(GetOwningActor());
}

const UCharacterBaseMovementComponent* UCharacterAnimInstance::GetCharacterMovement() const
{
	if (const ACharacterBase* Character = GetCharacter())
	{
		return Character->GetCharacterBaseMovement();
	}

	return nullptr;
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Initialize the property map
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningActor()))
	{
		GameplayTagPropertyMap.Initialize(this, ASC);
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// We need to get a reference to our custom character movement component, but we cannot use GetCharacterMovement
	// method defined above because it only supports returning the const pointer type. Thus, we need to work through the
	// logic again here to get the non-const pointer type.
	if (const ACharacterBase* Character = GetCharacter())
	{
		GroundDistance = Character->GetCharacterBaseMovement()->GetGroundInfo().GroundDistance;
	}
}

FAnimInstanceProxy* UCharacterAnimInstance::CreateAnimInstanceProxy()
{
	return new FCharacterAnimInstanceProxy(this);
}

void UCharacterAnimInstance::DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy)
{
	Super::DestroyAnimInstanceProxy(InProxy);
}

#if WITH_EDITOR
  EDataValidationResult UCharacterAnimInstance::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);
	GameplayTagPropertyMap.IsDataValid(this, ValidationErrors);
	return ValidationErrors.Num() > 0 ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif
