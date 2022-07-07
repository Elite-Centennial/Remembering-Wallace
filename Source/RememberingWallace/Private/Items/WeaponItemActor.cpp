// Elite Centennial

#include "Items/WeaponItemActor.h"

#include "ItemInstance.h"
#include "Items/EquipmentManagerComponent.h"
#include "Items/ItemProperty_Weapon.h"

AWeaponItemActor::AWeaponItemActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

const UItemProperty_Weapon* AWeaponItemActor::GetWeaponItemProperty() const
{
	if (const UItemInstance* Instance = GetItemInstance())
	{
		return Instance->GetProperty<UItemProperty_Weapon>();
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
		return Instance->GetPropertyData<UItemProperty_Weapon>();
	}

	return nullptr;
}

FItemPropertyData_Weapon* AWeaponItemActor::GetWeaponItemPropertyData()
{
	if (UItemInstance* Instance = GetItemInstance())
	{
		return Instance->GetPropertyData<UItemProperty_Weapon>();
	}

	return nullptr;
}

void AWeaponItemActor::Native_OnWeaponEquipped()
{
	const UItemProperty_Weapon* WeaponProperty = GetWeaponItemProperty();
	const FItemPropertyData_Weapon* WeaponData = GetWeaponItemPropertyData();
	check(WeaponProperty && WeaponData);

	const UEquipmentManagerComponent* EquipmentManager = WeaponData->EquipmentManager.Get();
	check(EquipmentManager); // Does not make sense to equip this weapon without the equipment manager

	if (USkeletalMeshComponent* SkeletalMeshComponent = EquipmentManager->GetSkeletalMeshComponent())
	{
		// Attach to the character's main skeletal mesh component
		AttachToComponent(
			SkeletalMeshComponent,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			WeaponProperty->GetSheathedSocketName());
	}
}

void AWeaponItemActor::Native_OnWeaponAboutToBeUnequipped(const bool bWillBeDestroyed)
{
	if (bWillBeDestroyed)
	{
		return;
	}

	const UItemProperty_Weapon* WeaponProperty = GetWeaponItemProperty();
	const FItemPropertyData_Weapon* WeaponData = GetWeaponItemPropertyData();
	check(WeaponProperty && WeaponData);

	const UEquipmentManagerComponent* EquipmentManager = WeaponData->EquipmentManager.Get();
	check(EquipmentManager); // This must still be valid because other unequip logics have not run yet

	if (const USkeletalMeshComponent* SkeletalMeshComponent = EquipmentManager->GetSkeletalMeshComponent())
	{
		// Detach from the character's main skeletal mesh component
		if (RootComponent->GetAttachParent() == SkeletalMeshComponent)
		{
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
	}
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
