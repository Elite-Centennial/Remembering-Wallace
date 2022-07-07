// Elite Centennial

#include "Player/WallacePlayerController.h"

#include "AbilitySystem/WallaceAbilitySystemComponent.h"
#include "Player/WallacePlayerState.h"

AWallacePlayerController::AWallacePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

AWallacePlayerState* AWallacePlayerController::GetWallacePlayerState() const
{
	return GetPlayerState<AWallacePlayerState>();
}

UWallaceAbilitySystemComponent* AWallacePlayerController::GetWallaceAbilitySystemComponent() const
{
	const AWallacePlayerState* WallacePS = GetWallacePlayerState();
	return WallacePS ? WallacePS->GetWallaceAbilitySystemComponent() : nullptr;
}

UEquipmentManagerComponent* AWallacePlayerController::GetEquipmentManagerComponent() const
{
	const AWallacePlayerState* WallacePS = GetWallacePlayerState();
	return WallacePS ? WallacePS->GetEquipmentManagerComponent() : nullptr;
}

UAbilitySystemComponent* AWallacePlayerController::GetAbilitySystemComponent() const
{
	return GetWallaceAbilitySystemComponent();
}

UInventoryComponent* AWallacePlayerController::GetInventoryComponent_Implementation() const
{
	const AWallacePlayerState* WallacePS = GetWallacePlayerState();
	return WallacePS ? Execute_GetInventoryComponent(WallacePS) : nullptr;
}
