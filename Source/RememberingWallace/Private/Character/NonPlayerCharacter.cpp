// Elite Centennial

#include "Character/NonPlayerCharacter.h"

#include "InventoryComponent.h"
#include "AbilitySystem/WallaceAbilitySystemComponent.h"
#include "Items/EquipmentManagerComponent.h"

const FName ANonPlayerCharacter::AbilitySystemComponentName(TEXT("AbilitySystemComponent"));
const FName ANonPlayerCharacter::InventoryComponentName(TEXT("InventoryComponent"));
const FName ANonPlayerCharacter::EquipmentManagerComponentName(TEXT("EquipmentManagerComponent"));

ANonPlayerCharacter::ANonPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UWallaceAbilitySystemComponent>(AbilitySystemComponentName);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(InventoryComponentName);

	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(EquipmentManagerComponentName);
}

void ANonPlayerCharacter::PossessedBy(AController* NewController)
{
	// Initialize various information on the ability system component and equipment manager component
	// This MUST come before calling the parent implementation because we want all data to be initialized before calling
	// the possessed event in the blueprint graph. However, we need to be careful that all fields related to the
	// information on the new "possessed" state are not assigned at all. Therefore, we should only use the data coming
	// in directly from the input value, the new controller actor.
	// We cannot use GetController here, so use the new controller directly.
	AbilitySystemComponent->InitAbilityActorInfo(NewController, this);

	// Handle the possession by the new controller
	Super::PossessedBy(NewController);
}
