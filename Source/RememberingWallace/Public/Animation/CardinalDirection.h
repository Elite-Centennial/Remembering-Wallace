// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CardinalDirection.generated.h"

/**
 * Enum of cardinal directions
 */
UENUM(BlueprintType, Category = "Animation")
enum class ECardinalDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right,
};

/**
 * A collection of animation sequences for each cardinal direction
 */
USTRUCT(BlueprintType)
struct FCardinalDirectionAnimationSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Forward = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Backward = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Left = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Right = nullptr;
};

/**
 * Blueprint function library for utility methods related to CardinalDirection enum
 */
UCLASS(meta = (BlueprintThreadSafe, ScriptName = "CardinalDirectionLibrary"))
class UCardinalDirectionBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Get the opposite direction of the given cardinal direction
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation|CardinalDirection")
	static ECardinalDirection GetOppositeDirection(ECardinalDirection Direction);

	/**
	 * Whether the two given cardinal directions are opposite to each other
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation|CardinalDirection")
	static bool IsOppositeDirection(ECardinalDirection Direction1, ECardinalDirection Direction2);

	/**
	 * Whether the two given cardinal directions are perpendicular to each other
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation|CardinalDirection")
	static bool IsPerpendicularDirection(ECardinalDirection Direction1, ECardinalDirection Direction2);

	/**
	 * Get the anim sequence corresponding to the given cardinal direction from the given animation set
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation|CardinalDirection")
	static UAnimSequence* SelectCardinalAnimSequence(
		const FCardinalDirectionAnimationSet& AnimSet, ECardinalDirection Direction);

	/**
	 * Select the most appropriate cardinal direction of the given yaw angle, optionally making it harder to break out
	 * from forward/backward range
	 *
	 * The YawAngle is assumed to be from -180 to 180 inclusive. If this is not ensured, you might get weird results.
	 *
	 * The logic is taken from Lyra Starter Game sample project.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation|CardinalDirection")
	static ECardinalDirection SelectDirectionFromYaw(
		float YawAngle,
		float ForwardBackwardBias = 0.0f,
		bool bUseCurrentDirection = false,
		ECardinalDirection CurrentDirection = ECardinalDirection::Forward);

	/**
	 * Return the direction vector of length 1 which represents the given direction
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation|CardinalDirection")
	static FVector GetDirectionVector(ECardinalDirection Direction);

	/**
	 * Returns the yaw angle of the given direction from -180 to 180 inclusive
	 *
	 * ECardinalDirection::Backward returns 180.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation|CardinalDirection")
	static float GetDirectionYawAngle(ECardinalDirection Direction);
};
