// Elite Centennial

#include "Items/EquipmentManagerComponent.h"

#include "AbilitySystemInterface.h"
#include "InventoryComponent.h"
#include "InventorySystemInterface.h"
#include "ItemInstance.h"
#include "RememberingWallace.h"
#include "AbilitySystem/WallaceAbilitySystemComponent.h"
#include "Items/ItemProperty_Weapon.h"
#include "Items/WeaponItemActor.h"

UEquipmentManagerComponent::UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Make the engine call the initialize method
	bWantsInitializeComponent = true;
}

void UEquipmentManagerComponent::EquipWeapon(
	bool& bChanged, UItemInstance*& PreviousWeapon, UItemInstance* NewWeapon, const bool bDestroyActor)
{
	// Set the default values for convenience
	PreviousWeapon = nullptr;
	bChanged = false;

	// If called on the same instance, don't do anything
	if (Weapon == NewWeapon)
	{
		return;
	}

	// If the new weapon is not in the inventory, don't do anything
	if (NewWeapon && InventoryComponent && !InventoryComponent->HasInstance(NewWeapon))
	{
		return;
	}

	// Fetch the weapon item property of the new weapon to check if it's a weapon and also use it later on
	const UItemProperty_Weapon* NewProperty = NewWeapon
		? NewWeapon->GetProperty<UItemProperty_Weapon>()
		: nullptr;

	// Error and don't proceed if the new item instance does not have weapon item property assigned to it
	if (NewWeapon && !NewProperty)
	{
		UE_LOG(LogWallace, Error,
			TEXT("EquipWeapon called on equipment manager of actor [%s] with an item instance [%s] of item [%s] without weapon property"),
			*GetNameSafe(GetOwner()), *GetNameSafe(NewWeapon), *GetNameSafe(NewWeapon->GetDefinition()));
		return;
	}

	// Assign the new weapon after saving the old one
	PreviousWeapon = Weapon;
	Weapon = NewWeapon;
	bChanged = true; // Indicate that the weapons are actually swapped; put here so that we don't forget

	// Clean up the data from previously equipped weapon if applicable
	if (PreviousWeapon)
	{
		if (const UItemProperty_Weapon* OldProperty = PreviousWeapon->GetProperty<UItemProperty_Weapon>())
		{
			OldProperty->Unequip(PreviousWeapon, bDestroyActor);
		}
	}

	// Equipping logic for the new weapon
	if (NewWeapon)
	{
		check(NewProperty); // Should be non-null due to one of the if blocks above

		// Execute any logics needed for equipping the weapon
		NewProperty->Equip(NewWeapon, this);
	}
}

bool UEquipmentManagerComponent::EquipWeapon(UItemInstance* NewWeapon, const bool bDestroyActor)
{
	bool bChanged;
	UItemInstance* PreviousWeapon;
	EquipWeapon(bChanged, PreviousWeapon, NewWeapon, bDestroyActor);
	return bChanged;
}

UItemInstance* UEquipmentManagerComponent::UnequipWeapon(const bool bDestroyActor)
{
	bool bChanged;
	UItemInstance* PreviousWeapon;
	EquipWeapon(bChanged, PreviousWeapon, nullptr, bDestroyActor);
	return PreviousWeapon;
}

AWeaponItemActor* UEquipmentManagerComponent::GetCurrentWeaponActor() const
{
	if (Weapon)
	{
		if (const FItemPropertyData_Weapon* WeaponData = Weapon->GetPropertyData<UItemProperty_Weapon>())
		{
			return WeaponData->SpawnedActor.Get();
		}
	}

	return nullptr;
}

AWeaponItemActor* UEquipmentManagerComponent::BP_GetCurrentWeaponActor(bool& bValid) const
{
	AWeaponItemActor* Actor = GetCurrentWeaponActor();
	bValid = Actor != nullptr;
	return Actor;
}

USkeletalMeshComponent* UEquipmentManagerComponent::GetSkeletalMeshComponent() const
{
	if (AbilitySystemComponent && AbilitySystemComponent->AbilityActorInfo.IsValid())
	{
		return AbilitySystemComponent->AbilityActorInfo->SkeletalMeshComponent.Get();
	}

	return nullptr;
}

void UEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	const AActor* OwningActor = GetOwner();

	// Cache the ASC
	if (const IAbilitySystemInterface* ASCOwner = Cast<IAbilitySystemInterface>(OwningActor))
	{
		AbilitySystemComponent = Cast<UWallaceAbilitySystemComponent>(ASCOwner->GetAbilitySystemComponent());
	}

	// Cache the inventory component
	if (OwningActor->Implements<UInventorySystemInterface>())
	{
		InventoryComponent = IInventorySystemInterface::Execute_GetInventoryComponent(OwningActor);
	}
}

void UEquipmentManagerComponent::UninitializeComponent()
{
	// Clean up the pointers; not sure if this is necessary but I want to be safe
	AbilitySystemComponent = nullptr;
	InventoryComponent = nullptr;

	Super::UninitializeComponent();
}

// TODO: Automatically un-equip any equipments when removed from the inventory
//       (needs inventory change events to be implemented in the inventory system)
