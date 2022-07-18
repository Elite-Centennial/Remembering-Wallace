// Elite Centennial

#include "Animation/CardinalDirection.h"

#include "RememberingWallace.h"

ECardinalDirection UCardinalDirectionBlueprintLibrary::GetOppositeDirection(const ECardinalDirection Direction)
{
	switch (Direction)
	{
	case ECardinalDirection::Forward:
		return ECardinalDirection::Backward;
	case ECardinalDirection::Backward:
		return ECardinalDirection::Forward;
	case ECardinalDirection::Left:
		return ECardinalDirection::Right;
	case ECardinalDirection::Right:
		return ECardinalDirection::Left;
	default:
		UE_LOG(LogWallace, Error, TEXT("Invalid ECardinalDirection received for GetOppositeDirection"));
		return ECardinalDirection::Forward;
	}
}

bool UCardinalDirectionBlueprintLibrary::IsOppositeDirection(
	const ECardinalDirection Direction1, const ECardinalDirection Direction2)
{
	return Direction1 == GetOppositeDirection(Direction2);
}

bool UCardinalDirectionBlueprintLibrary::IsPerpendicularDirection(
	const ECardinalDirection Direction1, const ECardinalDirection Direction2)
{
	// perpendicular == not parallel == neither equal nor opposite
	return Direction1 != Direction2 && !IsOppositeDirection(Direction1, Direction2);
}

UAnimSequence* UCardinalDirectionBlueprintLibrary::SelectCardinalAnimSequence(
	const FCardinalDirectionAnimationSet& AnimSet, const ECardinalDirection Direction)
{
	switch (Direction)
	{
	case ECardinalDirection::Forward:
		return AnimSet.Forward.Get();
	case ECardinalDirection::Backward:
		return AnimSet.Backward.Get();
	case ECardinalDirection::Left:
		return AnimSet.Left.Get();
	case ECardinalDirection::Right:
		return AnimSet.Right.Get();
	default:
		return nullptr;
	}
}

ECardinalDirection UCardinalDirectionBlueprintLibrary::SelectDirectionFromYaw(
	const float YawAngle,
	const float ForwardBackwardBias,
	const bool bUseCurrentDirection,
	const ECardinalDirection CurrentDirection)
{
	float ForwardBias = ForwardBackwardBias;
	float BackwardBias = ForwardBackwardBias;

	if (bUseCurrentDirection)
	{
		// Make it harder to break out of forward/backward range if already in that direction
		switch (CurrentDirection)
		{
		case ECardinalDirection::Forward:
			ForwardBias *= 2.0f;
			break;
		case ECardinalDirection::Backward:
			BackwardBias *= 2.0f;
			break;
		default:
			// Nothing to do
			break;
		}
	}

	const float YawAngleAbs = FMath::Abs(YawAngle);

	// Check the yaw angle to select an appropriate direction

	// Forward check
	if (YawAngleAbs <= 45.0f + ForwardBias)
	{
		return ECardinalDirection::Forward;
	}

	// Backward check
	if (YawAngleAbs >= 135.0f - BackwardBias)
	{
		return ECardinalDirection::Backward;
	}

	// Right/left check
	return YawAngle > 0 ? ECardinalDirection::Right : ECardinalDirection::Left;
}

FVector UCardinalDirectionBlueprintLibrary::GetDirectionVector(const ECardinalDirection Direction)
{
	switch (Direction)
	{
	case ECardinalDirection::Forward:
		return FVector::ForwardVector;
	case ECardinalDirection::Backward:
		return FVector::BackwardVector;
	case ECardinalDirection::Left:
		return FVector::LeftVector;
	case ECardinalDirection::Right:
		return FVector::RightVector;
	default:
		UE_LOG(LogWallace, Error, TEXT("Invalid ECardinalDirection received for GetOppositeDirection"));
		return FVector::ZeroVector;
	}
}

float UCardinalDirectionBlueprintLibrary::GetDirectionYawAngle(const ECardinalDirection Direction)
{
	switch (Direction)
	{
	case ECardinalDirection::Forward:
		return 0.0f;
	case ECardinalDirection::Backward:
		return 180.0f;
	case ECardinalDirection::Left:
		return -90.0f;
	case ECardinalDirection::Right:
		return 90.f;
	default:
		UE_LOG(LogWallace, Error, TEXT("Invalid ECardinalDirection received for GetOppositeDirection"));
		return 0.0f;
	}
}
