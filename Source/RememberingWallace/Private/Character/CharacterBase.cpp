// Elite Centennial

#include "Character/CharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"

ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
{
	// Character should rotate according to the move direction by default
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Weapon is sheathed by default
	WeaponState = ECharacterWeaponState::Sheathed;
}

void ACharacterBase::HandleWeaponGrabbed()
{
	if (WeaponState == ECharacterWeaponState::Drawing)
	{
		OnWeaponGrabbed();
	}
}

void ACharacterBase::HandleWeaponReleased()
{
	if (WeaponState == ECharacterWeaponState::Sheathing)
	{
		OnWeaponReleased();
	}
}

void ACharacterBase::HandleWeaponDrawingFinished()
{
	if (WeaponState != ECharacterWeaponState::Drawing)
	{
		return;
	}

	WeaponState = ECharacterWeaponState::Drawn;
}

void ACharacterBase::HandleWeaponSheathingFinished()
{
	if (WeaponState == ECharacterWeaponState::Sheathing)
	{
		WeaponState = ECharacterWeaponState::Sheathed;
	}
}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitASCActorInfo();
}

void ACharacterBase::RequestWeaponDraw()
{
	if (WeaponState != ECharacterWeaponState::Sheathed)
	{
		return;
	}

	WeaponState = ECharacterWeaponState::Drawing;

	// Character rotation should follow the camera when the weapon is drawn
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	if (WeaponDrawRequestedDelegate.IsBound())
	{
		WeaponDrawRequestedDelegate.Broadcast(this);
	}
}

void ACharacterBase::RequestWeaponSheath()
{
	if (WeaponState != ECharacterWeaponState::Drawn)
	{
		return;
	}

	WeaponState = ECharacterWeaponState::Sheathing;

	// Character should face the movement direction when the weapon is sheathed
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	if (WeaponSheatheRequestedDelegate.IsBound())
	{
		WeaponSheatheRequestedDelegate.Broadcast(this);
	}
}
