// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"

#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

/**
 * Character class for the player
 */
UCLASS()
class REMEMBERINGWALLACE_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

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

public:
	APlayerCharacter();

	// Cached names for the components
	static const FName CameraArmName;
	static const FName PlayerCameraName;

	/**
	 * Return the CameraArm sub-object
	 */
	FORCEINLINE USpringArmComponent* GetCameraArm() const { return CameraArm; }

	/**
	 * Return the PlayerCamera sub-object
	 */
	FORCEINLINE UCameraComponent* GetPlayerCamera() const { return PlayerCamera; }

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

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
};
