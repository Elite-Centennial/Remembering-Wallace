// Elite Centennial

#include "Character/PlayerCharacter.h"

#include "AbilitySystem/WallaceAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/EquipmentManagerComponent.h"
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

AWallacePlayerState* APlayerCharacter::GetWallacePlayerState() const
{
	return GetPlayerState<AWallacePlayerState>();
}

UInventoryComponent* APlayerCharacter::GetInventoryComponent_Implementation() const
{
	const AWallacePlayerState* WallacePS = GetWallacePlayerState();
	return WallacePS ? Execute_GetInventoryComponent(WallacePS) : nullptr;
}

UWallaceAbilitySystemComponent* APlayerCharacter::GetWallaceAbilitySystemComponent() const
{
	const AWallacePlayerState* WallacePS = GetWallacePlayerState();
	return WallacePS ? WallacePS->GetWallaceAbilitySystemComponent() : nullptr;
}

UEquipmentManagerComponent* APlayerCharacter::GetEquipmentManagerComponent() const
{
	const AWallacePlayerState* WallacePS = GetWallacePlayerState();
	return WallacePS ? WallacePS->GetEquipmentManagerComponent() : nullptr;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	// Initialize various information on the components in the player state
	// This MUST come before calling the parent implementation because we want all data to be initialized before calling
	// the possessed event in the blueprint graph. However, we need to be careful that all fields related to the
	// information on the new "possessed" state are not assigned at all. Therefore, we should only use the data coming
	// in directly from the input value, the new controller actor.
	// We cannot use GetPlayerState here, so we get the player state directly from the new controller.
	if (const AWallacePlayerState* WallacePS = Cast<AWallacePlayerState>(NewController->PlayerState))
	{
		// We cannot use GetController here, so use the new controller directly.
		WallacePS->GetAbilitySystemComponent()->InitAbilityActorInfo(NewController, this);
	}

	// Handle the possession by the new controller
	Super::PossessedBy(NewController);
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

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Directional movement bindings
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &APlayerCharacter::MoveRight);

	// Camera rotation bindings
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);

	// Register input bindings for the ASC
	// The ASC is never null here because we're doing a single-player game.
	GetAbilitySystemComponent()->BindToInputComponent(PlayerInputComponent);
}
