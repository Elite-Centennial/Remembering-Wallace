// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "CharacterBaseMovementComponent.generated.h"

class UCharacterBaseMovementComponent;

/**
 * Information about the ground trace of the character
 *
 * This code is adapted from the Lyra Starter Game sample project.
 */
USTRUCT(BlueprintType)
struct REMEMBERINGWALLACE_API FCharacterGroundInfo
{
	GENERATED_BODY()

	// Allow access to the private field
	friend UCharacterBaseMovementComponent;

	/**
	 * Whether this info is up to date
	 */
	bool IsUpToDate() const { return LastUpdateFrame == GFrameCounter; }

	/**
	 * Hit result of the physics tracing
	 */
	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	/**
	 * Distance to the ground
	 */
	UPROPERTY(BlueprintReadOnly)
	float GroundDistance = 0.0f;

private:
	/**
	 * The frame in which the information was last updated; used for caching
	 */
	uint64 LastUpdateFrame = 0;
};

/**
 * A custom character movement component used for this project
 *
 * There are mainly 2 reasons for this customization.
 * 1. Get the information about the ground under the character. This is especially helpful for a smooth landing motion.
 * 2. Disable movement when the owner has the gameplay tag that is meant to stop any movement.
 */
UCLASS()
class REMEMBERINGWALLACE_API UCharacterBaseMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UCharacterBaseMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Get the current ground info, updating the internally cached value only when necessary
	 *
	 * The code is taken from the Lyra Starter Game sample project.
	 */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement")
	const FCharacterGroundInfo& GetGroundInfo();

	/**
	 * Get the currently cached ground info
	 *
	 * To check whether returned value is up to date, use IsUpToDate method on the ground info.
	 */
	const FCharacterGroundInfo& GetCachedGroundInfo() const { return InternalCachedGroundInfo; }

	/**
	 * Get the currently cached ground info and if it is up to date
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn|Components|CharacterMovement",
		DisplayName = "Get Cached Ground Info")
	const FCharacterGroundInfo& BP_GetCachedGroundInfo(bool& bIsUpToDate) const;

	// BEGIN UCharacterMovementComponent interface
	virtual FRotator GetDeltaRotation(float DeltaTime) const override;
	// END UCharacterMovementComponent interface

	// BEGIN UMovementComponent interface
	virtual float GetMaxSpeed() const override;
	// END UMovementComponent interface

protected:
	/**
	 * Get the maximum distance to trace for the ground, set in the developer settings
	 *
	 * This is just a utility method that fetches the value.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn|Components|CharacterMovement")
	float GetGroundTraceDistance() const;

private:
	/**
	 * Cached ground info, not meant to be accessed directly
	 *
	 * To get the up-to-date information, use GetGroundInfo method.
	 */
	FCharacterGroundInfo InternalCachedGroundInfo;
};
