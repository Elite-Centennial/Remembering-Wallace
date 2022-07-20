// Elite Centennial

#include "Items/WeaponItemActor.h"

#include "ItemInstance.h"
#include "Items/EquipmentManagerComponent.h"
#include "Items/ItemProperty_Weapon.h"
#include "Items/WeaponState.h"

AWeaponItemActor::AWeaponItemActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

const UItemProperty_Weapon* AWeaponItemActor::GetWeaponItemProperty() const
{
	if (const UItemInstance* Instance = GetItemInstance())
	{
		return Instance->GetWeaponProperty();
	}

	return nullptr;
}

UEquipmentManagerComponent* AWeaponItemActor::GetEquipmentManager() const
{
	if (const FItemPropertyData_Weapon* Data = GetWeaponItemPropertyData())
	{
		return Data->EquipmentManager.Get();
	}

	return nullptr;
}

const FItemPropertyData_Weapon* AWeaponItemActor::GetWeaponItemPropertyData() const
{
	if (const UItemInstance* Instance = GetItemInstance())
	{
		return Instance->GetWeaponPropertyData();
	}

	return nullptr;
}

FItemPropertyData_Weapon* AWeaponItemActor::GetWeaponItemPropertyData()
{
	if (UItemInstance* Instance = GetItemInstance())
	{
		return Instance->GetWeaponPropertyData();
	}

	return nullptr;
}

bool AWeaponItemActor::AttachToOwnerCharacter(const EWeaponState Position)
{
	if (const UEquipmentManagerComponent* EquipmentManager = GetEquipmentManager())
	{
		if (USkeletalMeshComponent* SkeletalMeshComponent = EquipmentManager->GetSkeletalMeshComponent())
		{
			// Attach to the character's main skeletal mesh component
			// We already know that weapon item property is not null because the equipment manager is not null which in
			// turn means that weapon property data is not null.
			AttachToComponent(
				SkeletalMeshComponent,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				GetWeaponItemProperty()->GetActorPositionSocketName(Position));
			return true;
		}
	}

	return false;
}

bool AWeaponItemActor::DetachFromOwnerCharacter()
{
	if (const UEquipmentManagerComponent* EquipmentManager = GetEquipmentManager())
	{
		if (const USkeletalMeshComponent* SkeletalMeshComponent = EquipmentManager->GetSkeletalMeshComponent())
		{
			// Detach from the character's main skeletal mesh component
			if (SkeletalMeshComponent == RootComponent->GetAttachParent())
			{
				DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				return true;
			}
		}
	}

	return false;
}

void AWeaponItemActor::Native_OnWeaponEquipped()
{
	EWeaponState CurrentWeaponState = EWeaponState::Sheathed;

	if (const UEquipmentManagerComponent* EquipmentManager = GetEquipmentManager())
	{
		CurrentWeaponState = EquipmentManager->GetCurrentWeaponState();
	}

	AttachToOwnerCharacter(CurrentWeaponState);
}

void AWeaponItemActor::Native_OnWeaponAboutToBeUnequipped(const bool bWillBeDestroyed)
{
	if (bWillBeDestroyed)
	{
		return;
	}

	DetachFromOwnerCharacter();
}

void AWeaponItemActor::OnWeaponEquipped()
{
	Native_OnWeaponEquipped();
	BP_OnWeaponEquipped();
}

void AWeaponItemActor::OnWeaponAboutToBeUnequipped(const bool bWillBeDestroyed)
{
	// Call the blueprint event first so that the native code can do some low-level clean-ups if wanted
	BP_OnWeaponAboutToBeUnequipped(bWillBeDestroyed);
	Native_OnWeaponAboutToBeUnequipped(bWillBeDestroyed);
}
