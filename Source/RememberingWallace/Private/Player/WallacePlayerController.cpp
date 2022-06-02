// Elite Centennial

#include "Player/WallacePlayerController.h"

#include "AbilitySystem/WallaceAbilitySystemComponent.h"
#include "Player/WallacePlayerState.h"

AWallacePlayerController::AWallacePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UWallaceAbilitySystemComponent* AWallacePlayerController::GetWallaceAbilitySystemComponent() const
{
	if (const AWallacePlayerState* WallacePS = GetPlayerState<AWallacePlayerState>())
	{
		return WallacePS->GetWallaceAbilitySystemComponent();
	}

	return nullptr;
}

UAbilitySystemComponent* AWallacePlayerController::GetAbilitySystemComponent() const
{
	return GetWallaceAbilitySystemComponent();
}
