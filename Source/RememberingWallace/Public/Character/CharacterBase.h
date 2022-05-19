// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CharacterBase.generated.h"

class ACharacterBase;

/**
 * Event dispatcher for when weapon draw is requested
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterWeaponDrawRequested, const ACharacterBase*, Character);

/**
 * Event dispatcher for when weapon sheathe is requested
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterWeaponSheatheRequested, const ACharacterBase*, Character);

UENUM(BlueprintType, Category = "Weapon")
enum class ECharacterWeaponState : uint8
{
	Sheathed,
	Drawn,
	Drawing,
	Sheathing
};

/**
 * Base character class for all character classes
 *
 * This class mostly consists of common functionalities such as combat.
 */
UCLASS()
class REMEMBERINGWALLACE_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

	/**
	 * State of the weapon (drawn, sheathed, or in transition)
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	ECharacterWeaponState WeaponState;

	/**
	 * Event for when a request to draw the weapon is successfully made
	 *
	 * This is intended to be bound with a function that plays an animation montage.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Weapon", DisplayName = "On Weapon Draw Requested",
		meta = (AllowPrivateAccess = "true"))
	FCharacterWeaponDrawRequested WeaponDrawRequestedDelegate;

	/**
	 * Event for when a request to sheathe the weapon is successfully made
	 *
	 * This is intended to be bound with a function that plays an animation montage.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Weapon", DisplayName = "On Weapon Sheathe Requested",
		meta = (AllowPrivateAccess = "true"))
	FCharacterWeaponSheatheRequested WeaponSheatheRequestedDelegate;

public:
	ACharacterBase();

	/**
	 * Return the weapon state
	 */
	FORCEINLINE ECharacterWeaponState GetWeaponState() const { return WeaponState; }

	/**
	 * Return the delegate for when weapon draw is successfully requested
	 */
	FORCEINLINE FCharacterWeaponDrawRequested& OnWeaponDrawRequested() { return WeaponDrawRequestedDelegate; }

	/**
	 * Return the delegate for when weapon sheathe is successfully requested
	 */
	FORCEINLINE FCharacterWeaponSheatheRequested& OnWeaponSheathRequested() { return WeaponSheatheRequestedDelegate; }

	/**
	 * Handle the event where the weapon is grabbed while drawing it out
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void HandleWeaponGrabbed();

	/**
	 * Handle the event where the weapon is released while sheathing it
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void HandleWeaponReleased();

	/**
	 * Handle the event where transition from sheathed to drawn is finished
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void HandleWeaponDrawingFinished();

	/**
	 * Handle the event where transition from drawn to sheathed is finished
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void HandleWeaponSheathingFinished();

protected:
	/**
	 * Request to start drawing the weapon
	 *
	 * This signals `WeaponDrawRequestedDelegate` only if the weapon state is `Sheathed`.
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon", meta = (BlueprintProtected = "true"))
	void RequestWeaponDraw();

	/**
	 * Request to start sheathing the weapon
	 *
	 * This signals `WeaponSheatheRequestedDelegate` only if the weapon state is `Drawn`.
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon", meta = (BlueprintProtected = "true"))
	void RequestWeaponSheath();

	/**
	 * Called when the weapon should be grabbed while drawing it out
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnWeaponGrabbed();

	/**
	* Called when the weapon should be release while sheathing it
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnWeaponReleased();
};
