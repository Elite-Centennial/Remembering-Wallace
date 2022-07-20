// Elite Centennial

#include "Player/WallacePlayerState.h"

#include "InventoryComponent.h"
#include "AbilitySystem/WallaceAbilitySystemComponent.h"
#include "Items/EquipmentManagerComponent.h"

const FName AWallacePlayerState::AbilitySystemComponentName(TEXT("AbilitySystemComponent"));
const FName AWallacePlayerState::InventoryComponentName(TEXT("InventoryComponent"));
const FName AWallacePlayerState::EquipmentManagerComponentName(TEXT("EquipmentManagerComponent"));

AWallacePlayerState::AWallacePlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UWallaceAbilitySystemComponent>(AbilitySystemComponentName);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(InventoryComponentName);

	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(EquipmentManagerComponentName);
}

UAbilitySystemComponent* AWallacePlayerState::GetAbilitySystemComponent() const
{
	return GetWallaceAbilitySystemComponent();
}
