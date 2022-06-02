// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Character/CharacterBase.h"

#include "PlayerCharacter.generated.h"

class AWallacePlayerState;
class UCameraComponent;
class USpringArmComponent;
class UWallaceAbilitySystemComponent;

/**
 * Character class for the player
 */
UCLASS()
class REMEMBERINGWALLACE_API APlayerCharacter : public ACharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Cached names for the components
	static const FName CameraArmName;
	static const FName PlayerCameraName;

	/**
	 * Return the CameraArm sub-object
	 */
	USpringArmComponent* GetCameraArm() const { return CameraArm; }

	/**
	 * Return the PlayerCamera sub-object
	 */
	UCameraComponent* GetPlayerCamera() const { return PlayerCamera; }

	/**
	 * Return the associated ASC from the player state as our customized class
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AbilitySystem")
	UWallaceAbilitySystemComponent* GetWallaceAbilitySystemComponent() const;

	// BEGIN IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// END IAbilitySystemInterface interface

protected:
	/**
	 * Callback for player input: forward/backward movement
	 *
	 * @param Value Input value from -1.0 to 1.0 inclusive
	 */
	void MoveForward(float Value);

	/**
	 * Callback for player input: right/left movement
	 *
	 * @param Value Input value from -1.0 to 1.0 inclusive
	 */
	void MoveRight(float Value);

	/**
	 * Callback for player input: toggle weapon draw/sheathe state
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon", meta = (BlueprintProtected = "true"))
	void ToggleWeaponDrawSheathe();

	// BEGIN ACharacterBase interface
	virtual void InitASCActorInfo() override;
	// END ACharacterBase interface

	// BEGIN APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// END APawn interface

private:
	/**
	 * Arm for the camera that avoids any obstacles in the vision
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraArm;

	/**
	 * Camera that follows the character
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCamera;
};
