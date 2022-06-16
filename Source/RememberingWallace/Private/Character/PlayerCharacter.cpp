// Elite Centennial

#include "Character/PlayerCharacter.h"

#include "AbilitySystem/WallaceAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/WallacePlayerState.h"

const FName APlayerCharacter::CameraArmName(TEXT("CameraArm"));
const FName APlayerCharacter::PlayerCameraName(TEXT("PlayerCamera"));

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Create CameraArm component
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(CameraArmName);
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 400.0f;
	// Rotate the camera arm along with the controller
	CameraArm->bUsePawnControlRotation = true;

	// Create PlayerCamera component
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(PlayerCameraName);
	PlayerCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	// Rotation is already handled by CameraArm
	PlayerCamera->bUsePawnControlRotation = false;
}

UWallaceAbilitySystemComponent* APlayerCharacter::GetWallaceAbilitySystemComponent() const
{
	if (const AWallacePlayerState* WallacePS = GetPlayerState<AWallacePlayerState>())
	{
		return WallacePS->GetWallaceAbilitySystemComponent();
	}

	return nullptr;
}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return GetWallaceAbilitySystemComponent();
}

void APlayerCharacter::MoveForward(const float Value)
{
	if (Controller == nullptr || Value == 0.0f)
	{
		return;
	}

	// Get forward direction
	const FRotator CurrentRotation = Controller->GetControlRotation();
	const FRotator Yaw(0, CurrentRotation.Yaw, 0);
	const FVector Forward = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);

	// Apply input
	AddMovementInput(Forward, Value);
}

void APlayerCharacter::MoveRight(const float Value)
{
	if (Controller == nullptr || Value == 0.0f)
	{
		return;
	}

	// Get right-ward direction
	const FRotator CurrentRotation = Controller->GetControlRotation();
	const FRotator Yaw(0, CurrentRotation.Yaw, 0);
	const FVector Right = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);

	// Apply input
	AddMovementInput(Right, Value);
}

void APlayerCharacter::ToggleWeaponDrawSheathe()
{
	switch (GetWeaponState())
	{
	case ECharacterWeaponState::Sheathed:
		RequestWeaponDraw();
		break;
	case ECharacterWeaponState::Drawn:
		RequestWeaponSheath();
		break;
	default:
		break;
	}
}

void APlayerCharacter::InitASCActorInfo()
{
	if (AWallacePlayerState* WallacePS = GetPlayerState<AWallacePlayerState>())
	{
		WallacePS->GetAbilitySystemComponent()->InitAbilityActorInfo(WallacePS, this);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Directional movement bindings
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &APlayerCharacter::MoveRight);

	// Camera rotation bindings
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);

	// Jump action bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Weapon draw / sheathe action bindings
	PlayerInputComponent->BindAction(
		"Weapon Draw / Sheath", IE_Pressed, this, &APlayerCharacter::ToggleWeaponDrawSheathe);
}
