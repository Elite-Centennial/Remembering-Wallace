// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "InventorySystemInterface.h"
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
class REMEMBERINGWALLACE_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Cached names for the components
	static const FName CameraArmName;
	static const FName PlayerCameraName;

	/**
	 * Constructor
	 */
	APlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Return the CameraArm sub-object
	 */
	USpringArmComponent* GetCameraArm() const { return CameraArm; }

	/**
	 * Return the PlayerCamera sub-object
	 */
	UCameraComponent* GetPlayerCamera() const { return PlayerCamera; }

	/**
	 * Return the player state as the class used in this project
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn")
	AWallacePlayerState* GetWallacePlayerState() const;

	// BEGIN IInventorySystemInterface interface
	virtual UInventoryComponent* GetInventoryComponent_Implementation() const override;
	// END IInventorySystemInterface interface

	// BEGIN ACharacterBase interface
	virtual UWallaceAbilitySystemComponent* GetWallaceAbilitySystemComponent() const override;
	virtual UEquipmentManagerComponent* GetEquipmentManager() const override;
	// END ACharacterBase interface

	// BEGIN APawn interface
	virtual void PossessedBy(AController* NewController) override;
	// END APawn interface

protected:
	/**
	 * Callback for player input: forward/backward movement
	 *
	 * @param Value Input value from -1.0 to 1.0 inclusive
	 */
	UFUNCTION(BlueprintCallable, Category = "Character", meta = (BlueprintProtected = "true"))
	void MoveForward(float Value);

	/**
	 * Callback for player input: right/left movement
	 *
	 * @param Value Input value from -1.0 to 1.0 inclusive
	 */
	UFUNCTION(BlueprintCallable, Category = "Character", meta = (BlueprintProtected = "true"))
	void MoveRight(float Value);

	// BEGIN APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// END APawn interface

private:
	/**
	 * Arm for the camera that avoids any obstacles in the vision
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraArm;

	/**
	 * Camera that follows the character
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> PlayerCamera;
};
