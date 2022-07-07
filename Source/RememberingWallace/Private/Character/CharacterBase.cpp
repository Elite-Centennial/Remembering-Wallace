// Elite Centennial

#include "Character/CharacterBase.h"

#include "AbilitySystem/WallaceAbilitySystemComponent.h"
#include "Character/CharacterBaseMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
		.SetDefaultSubobjectClass<UCharacterBaseMovementComponent>(CharacterMovementComponentName))
{
	// Character should rotate according to the move direction by default
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

UCharacterBaseMovementComponent* ACharacterBase::GetCharacterBaseMovement() const
{
	return CastChecked<UCharacterBaseMovementComponent>(GetCharacterMovement());
}

void ACharacterBase::SetFollowControllerRotation(const bool bFollowControllerRotation)
{
	bUseControllerRotationYaw = bFollowControllerRotation;
	GetCharacterMovement()->bOrientRotationToMovement = !bFollowControllerRotation;
}

void ACharacterBase::HandleWeaponGrabbed()
{
	OnWeaponGrabbed();
}

void ACharacterBase::HandleWeaponReleased()
{
	OnWeaponReleased();
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return GetWallaceAbilitySystemComponent();
}
