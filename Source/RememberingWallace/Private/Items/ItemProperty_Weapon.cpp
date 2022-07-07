// Elite Centennial

#include "Items/ItemProperty_Weapon.h"

#include "ItemDefinition.h"
#include "ItemInstance.h"
#include "RememberingWallace.h"
#include "AbilitySystem/WallaceAbilitySystemComponent.h"
#include "Development/DeveloperSettings_Equipment.h"
#include "Items/EquipmentManagerComponent.h"
#include "Items/WeaponItemActor.h"

UItemProperty_Weapon::UItemProperty_Weapon()
{
}

TSubclassOf<UAnimInstance> UItemProperty_Weapon::GetAnimClassToLink(const USkeleton* Skeleton) const
{
	if (const TSubclassOf<UAnimInstance>* AnimClassOverride = AnimClassToLinkOverrides.Find(Skeleton))
	{
		// TODO: null check?
		return *AnimClassOverride;
	}

	const UDeveloperSettings_Equipment* EquipmentSettings = GetDefault<UDeveloperSettings_Equipment>();
	check(EquipmentSettings);

	if (const auto* WeaponTypeToAnimClassMap = EquipmentSettings->AnimClassToLinkPerWeaponType.Find(Skeleton))
	{
		if (const TSoftClassPtr<UAnimInstance>* AnimClass = WeaponTypeToAnimClassMap->Map.Find(WeaponType))
		{
			return AnimClass->LoadSynchronous();
		}
	}

	return nullptr;
}

void UItemProperty_Weapon::Equip(UItemInstance* WeaponInstance, UEquipmentManagerComponent* EquipmentManager) const
{
	check(WeaponInstance);
	check(EquipmentManager);

	// Get the weapon property data saved in the item instance
	FItemPropertyData_Weapon* InstanceData = WeaponInstance->GetPropertyData<UItemProperty_Weapon>();

	// If the item instance is a weapon, it MUST have a weapon property data (see CreateNewDataInstance method below)
	if (!InstanceData)
	{
		UE_LOG(LogWallace, Error,
			TEXT("Weapon equip called on an item instance [%s] of item [%s] without a weapon instance data"),
			*GetNameSafe(WeaponInstance), *GetNameSafe(WeaponInstance->GetDefinition()));
		return;
	}

	// Set the equipment manager component
	InstanceData->EquipmentManager = EquipmentManager;

	// The place to save the spawned weapon actor
	// Try to reuse the existing actor if it exists
	AWeaponItemActor* SpawnedActor = InstanceData->SpawnedActor.Get();

	// Spawn the weapon actor
	if (ActorToSpawn && EquipmentManager->GetWorld())
	{
		// Spawn a new actor if one doesn't exist
		if (!SpawnedActor)
		{
			// Set options for spawning the actor
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = EquipmentManager->GetOwner();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Set the instigator pawn if able to
			if (EquipmentManager->GetAbilitySystemComponent())
			{
				SpawnParams.Instigator = Cast<APawn>(EquipmentManager->GetAbilitySystemComponent()->GetAvatarActor());
			}

			// Spawn the weapon actor
			SpawnedActor = EquipmentManager->GetWorld()->SpawnActor<AWeaponItemActor>(ActorToSpawn, SpawnParams);
		}

		if (SpawnedActor)
		{
			// Assign data to the spawned actor
			SpawnedActor->SetItemInstance(WeaponInstance);

			// Save the spawned actor to the item instance data
			InstanceData->SpawnedActor = SpawnedActor;
		}
		else
		{
			// Log if failed to spawn the actor
			UE_LOG(LogWallace, Error,
				TEXT("Failed to spawn weapon actor of class [%s] for weapon instance [%s]"),
				*GetNameSafe(ActorToSpawn), *GetNameSafe(WeaponInstance));
		}
	}

	// Apply the ability set
	if (AbilitySet && EquipmentManager->GetAbilitySystemComponent())
	{
		// Clean up the ability set's granted handles saved in this weapon instance just in case
		// If there is data for a different (and valid) ASC, UAbilitySet::Grant does not do anything.
		InstanceData->GrantedHandles.TakeBack();

		// Grant the ability set to the weapon owner's ASC
		AbilitySet->Grant(EquipmentManager->GetAbilitySystemComponent(), &InstanceData->GrantedHandles);
	}

	// Notify the spawned actor that the weapon is equipped
	// This SHOULD come last to provide a good guarantee on when this event is called on the actor.
	if (SpawnedActor)
	{
		SpawnedActor->OnWeaponEquipped();
	}
}

void UItemProperty_Weapon::Unequip(UItemInstance* WeaponInstance, const bool bDestroyActor) const
{
	check(WeaponInstance);

	// Get the weapon property data saved in the item instance
	FItemPropertyData_Weapon* InstanceData = WeaponInstance->GetPropertyData<UItemProperty_Weapon>();

	// If the item instance is a weapon, it MUST have a weapon property data (see CreateNewDataInstance method below)
	if (!InstanceData)
	{
		UE_LOG(LogWallace, Error,
			TEXT("Weapon unequip called on an item instance [%s] of item [%s] without a weapon instance data"),
			*GetNameSafe(WeaponInstance), *GetNameSafe(WeaponInstance->GetDefinition()));
		return;
	}

	// Get the spawned actor
	AWeaponItemActor* SpawnedActor = InstanceData->SpawnedActor.Get();

	// Notify the actor of the unequip operation if it exists
	if (SpawnedActor)
	{
		SpawnedActor->OnWeaponAboutToBeUnequipped(bDestroyActor);
	}

	// Take back the granted ability set
	InstanceData->GrantedHandles.TakeBack();

	// Destroy the actor if told to do so
	if (bDestroyActor && SpawnedActor)
	{
		SpawnedActor->Destroy();
		InstanceData->SpawnedActor.Reset();
	}

	// Reset the pointer to the equipment manager
	InstanceData->EquipmentManager.Reset();
}

FItemPropertyData* UItemProperty_Weapon::CreateNewDataInstance() const
{
	return new FItemPropertyData_Weapon();
}
