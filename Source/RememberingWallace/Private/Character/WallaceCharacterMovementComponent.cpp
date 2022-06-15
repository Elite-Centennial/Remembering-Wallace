// Elite Centennial

#include "Character/WallaceCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/CapsuleComponent.h"
#include "Development/DeveloperSettings_Characters.h"
#include "GameFramework/Character.h"

DEFINE_GAMEPLAY_TAG(GTag_Unit_State_Immobilized, "Unit.State.Immobilized",
	TEXT("Movement is prohibited in this state"));

UWallaceCharacterMovementComponent::UWallaceCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const FCharacterGroundInfo& UWallaceCharacterMovementComponent::GetGroundInfo()
{
	// Do not compute if the actor isn't valid or the info is already up to date
	if (!CharacterOwner || InternalCachedGroundInfo.IsUpToDate())
	{
		return InternalCachedGroundInfo;
	}

	// We're updating the info in the code below, so set this first. That way, we don't have to worry about making sure
	// this is updated before doing an early exit from the function. This only works if all exit paths actually modify
	// the whole info.
	InternalCachedGroundInfo.LastUpdateFrame = GFrameCounter;

	// Use the already-computed info when walking
	if (MovementMode == MOVE_Walking)
	{
		InternalCachedGroundInfo.GroundHitResult = CurrentFloor.HitResult;
		InternalCachedGroundInfo.GroundDistance = 0.0f;
		return InternalCachedGroundInfo;
	}

	// Manual tracing to update the info

	// Capsule component of the character actor
	const UCapsuleComponent* CapsuleComponent = CharacterOwner->GetCapsuleComponent();
	check(CapsuleComponent);

	// The maximum value for the trace distance
	const float GroundTraceDistance = GetGroundTraceDistance();

	// Setup start and end for tracing; starts from the center and ends at a point below by the trace distance from the
	// bottom of the capsule
	const float CapsuleHalfHeight = CapsuleComponent->GetUnscaledCapsuleHalfHeight();
	const FVector TraceStart(GetActorLocation());
	const float TraceEndZ = TraceStart.Z - CapsuleHalfHeight - GroundTraceDistance;
	const FVector TraceEnd(TraceStart.X, TraceStart.Y, TraceEndZ);

	// Default to the pawn collision channel
	const ECollisionChannel CollisionChannel = UpdatedComponent ? UpdatedComponent->GetCollisionObjectType() : ECC_Pawn;

	// Prepare params for tracing
	FCollisionQueryParams QueryParams(
		SCENE_QUERY_STAT(WallaceCharacterMovementComponent_GetGroundInfo), false, CharacterOwner);
	FCollisionResponseParams ResponseParams;
	InitCollisionParams(QueryParams, ResponseParams);

	// Do the trace and set the hit result
	FHitResult& HitResult = InternalCachedGroundInfo.GroundHitResult; // Alias
	GetWorld()->LineTraceSingleByChannel(
		HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParams, ResponseParams);

	// Update the distance info
	if (MovementMode == MOVE_NavWalking)
	{
		InternalCachedGroundInfo.GroundDistance = 0.0f;
	}
	else if (HitResult.bBlockingHit)
	{
		// We need to subtract the half height because the trace started at the center of the capsule
		InternalCachedGroundInfo.GroundDistance = FMath::Max(HitResult.Distance - CapsuleHalfHeight, 0.0f);
	}
	else
	{
		// Maximum value in case there was no hit
		InternalCachedGroundInfo.GroundDistance = GroundTraceDistance;
	}

	return InternalCachedGroundInfo;
}

const FCharacterGroundInfo& UWallaceCharacterMovementComponent::BP_GetCachedGroundInfo(bool& bIsUpToDate) const
{
	bIsUpToDate = InternalCachedGroundInfo.IsUpToDate();
	return InternalCachedGroundInfo;
}

FRotator UWallaceCharacterMovementComponent::GetDeltaRotation(const float DeltaTime) const
{
	// Return zero if the owning actor has the immobilized tag
	if (const UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if (ASC->HasMatchingGameplayTag(GTag_Unit_State_Immobilized))
		{
			return FRotator::ZeroRotator;
		}
	}

	return Super::GetDeltaRotation(DeltaTime);
}

float UWallaceCharacterMovementComponent::GetMaxSpeed() const
{
	// Return zero if the owning actor has the immobilized tag
	if (const UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if (ASC->HasMatchingGameplayTag(GTag_Unit_State_Immobilized))
		{
			return 0.0f;
		}
	}

	return Super::GetMaxSpeed();
}

float UWallaceCharacterMovementComponent::GetGroundTraceDistance() const
{
	return GetDefault<UDeveloperSettings_Characters>()->GroundTraceDistance;
}
