// Elite Centennial

#include "Player/WallacePlayerState.h"

#include "AbilitySystem/WallaceAbilitySystemComponent.h"

const FName AWallacePlayerState::AbilitySystemComponentName(TEXT("AbilitySystemComponent"));

AWallacePlayerState::AWallacePlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UWallaceAbilitySystemComponent>(AbilitySystemComponentName);
}

UAbilitySystemComponent* AWallacePlayerState::GetAbilitySystemComponent() const
{
	return GetWallaceAbilitySystemComponent();
}
