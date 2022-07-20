// Elite Centennial

#include "AbilitySystem/Abilities/GameplayAbilityBase.h"

#include "InventorySystemInterface.h"
#include "AbilitySystem/WallaceAbilitySystemComponent.h"
#include "Character/CharacterBase.h"
#include "GameFramework/Actor.h"

UGameplayAbilityBase::UGameplayAbilityBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bActivateOnInput = true;
}

ACharacterBase* UGameplayAbilityBase::GetCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<ACharacterBase>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

UWallaceAbilitySystemComponent* UGameplayAbilityBase::GetWallaceAbilitySystemComponentFromActorInfo() const
{
	return CurrentActorInfo
		? Cast<UWallaceAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get())
		: nullptr;
}

UInventoryComponent* UGameplayAbilityBase::GetInventoryComponentFromActorInfo() const
{
	// Current implementation tries to get the inventory component from the owner actor and then the avatar actor on
	// demand. In case we need to optimize this part, we can subclass FGameplayAbilityActorInfo and add a field for
	// inventory component. To use that new type, we need to override UAbilitySystemGlobals::AllocAbilityActorInfo.

	if (CurrentActorInfo)
	{
		// Try to get the inventory component from the owner
		if (const AActor* OwnerActor = CurrentActorInfo->OwnerActor.Get())
		{
			if (OwnerActor->Implements<UInventorySystemInterface>())
			{
				// NOTE(Dohyun): Should we check for null here and only return when not null?
				return IInventorySystemInterface::Execute_GetInventoryComponent(OwnerActor);
			}
		}

		// If failed, try to get it from the avatar
		if (const AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get())
		{
			if (AvatarActor->Implements<UInventorySystemInterface>())
			{
				return IInventorySystemInterface::Execute_GetInventoryComponent(AvatarActor);
			}
		}
	}

	return nullptr;
}

UEquipmentManagerComponent* UGameplayAbilityBase::GetEquipmentManagerFromActorInfo() const
{
	// Current implementation tries to get the equipment manager component from the avatar actor on demand. In case we
	// need to optimize this part, we can subclass FGameplayAbilityActorInfo and add a field for equipment manager
	// component. To use that new type, we need to override UAbilitySystemGlobals::AllocAbilityActorInfo.

	if (CurrentActorInfo)
	{
		if (const ACharacterBase* Character = Cast<ACharacterBase>(CurrentActorInfo->AvatarActor.Get()))
		{
			return Character->GetEquipmentManager();
		}
	}

	return nullptr;
}
