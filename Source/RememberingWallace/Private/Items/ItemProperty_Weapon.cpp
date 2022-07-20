// Elite Centennial

#include "Items/ItemProperty_Weapon.h"

#include "ItemDefinition.h"
#include "ItemInstance.h"
#include "RememberingWallace.h"
#include "AbilitySystem/WallaceAbilitySystemComponent.h"
#include "Development/ProjectSettings_Equipment.h"
#include "Items/EquipmentManagerComponent.h"
#include "Items/WeaponItemActor.h"

namespace RememberingWallace::Weapon
{
	/**
	 * Internal helper function that retrieves the default settings for the given weapon type; can be null
	 */
	const FWeaponDefaultSettings* GetDefaultSettings(const FGameplayTag& WeaponType)
	{
		return UProjectSettings_Equipment::Get()->WeaponDefaultSettings.Find(WeaponType);
	}
}

UItemProperty_Weapon::UItemProperty_Weapon()
{
}

FName UItemProperty_Weapon::GetActorPositionSocketName(const EWeaponState Position) const
{
	FName SocketName = ActorSocketNamesOverride.GetSocketNameForPosition(Position);

	if (SocketName.IsNone())
	{
		if (const FWeaponDefaultSettings* Settings = RememberingWallace::Weapon::GetDefaultSettings(WeaponType))
		{
			SocketName = Settings->SocketNames.GetSocketNameForPosition(Position);
		}
	}

	return MoveTemp(SocketName);
}

UAnimMontage* UItemProperty_Weapon::GetDrawMontage() const
{
	if (DrawMontageOverride)
	{
		return DrawMontageOverride;
	}

	if (const FWeaponDefaultSettings* Settings = RememberingWallace::Weapon::GetDefaultSettings(WeaponType))
	{
		return Settings->DrawMontage.LoadSynchronous();
	}

	return nullptr;
}

UAnimMontage* UItemProperty_Weapon::GetSheatheMontage() const
{
	if (SheatheMontageOverride)
	{
		return SheatheMontageOverride;
	}

	if (const FWeaponDefaultSettings* Settings = RememberingWallace::Weapon::GetDefaultSettings(WeaponType))
	{
		return Settings->SheatheMontage.LoadSynchronous();
	}

	return nullptr;
}

TSubclassOf<UAnimInstance> UItemProperty_Weapon::GetAnimClassToLink() const
{
	if (AnimClassToLinkOverride)
	{
		return AnimClassToLinkOverride;
	}

	if (const FWeaponDefaultSettings* Settings = RememberingWallace::Weapon::GetDefaultSettings(WeaponType))
	{
		return Settings->AnimClassToLink.LoadSynchronous();
	}

	return nullptr;
}

void UItemProperty_Weapon::Equip(UItemInstance* WeaponInstance, UEquipmentManagerComponent* EquipmentManager) const
{
	check(WeaponInstance);
	check(EquipmentManager);

	// Get the weapon property data saved in the item instance
	FItemPropertyData_Weapon* InstanceData = WeaponInstance->GetWeaponPropertyData();

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
	if (!AbilitySets.IsEmpty() && EquipmentManager->GetAbilitySystemComponent())
	{
		// Clean up the ability set's granted handles saved in this weapon instance just in case
		InstanceData->CleanUpGrantedHandles();
		InstanceData->GrantedHandles.AddDefaulted(AbilitySets.Num());

		// Grant the ability sets to the weapon owner's ASC
		for (int32 Idx = 0; Idx < AbilitySets.Num(); Idx++)
		{
			AbilitySets[Idx]->Grant(EquipmentManager->GetAbilitySystemComponent(), &InstanceData->GrantedHandles[Idx]);
		}
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
	FItemPropertyData_Weapon* InstanceData = WeaponInstance->GetWeaponPropertyData();

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

	// Take back the granted ability sets
	InstanceData->CleanUpGrantedHandles();

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

void FItemPropertyData_Weapon::CleanUpGrantedHandles()
{
	for (FAbilitySetGrantedHandles& Handles : GrantedHandles)
	{
		Handles.TakeBack();
	}

	GrantedHandles.Reset();
}
