// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "Actors/ItemInstanceActor.h"

#include "WeaponItemActor.generated.h"

class UEquipmentManagerComponent;
class UItemProperty_Weapon;
class UItemDefinition;
class UItemInstance;
class UItemProperty_Weapon;
struct FItemPropertyData_Weapon;

/**
 * Base class for all actors that are spawned for a weapon
 */
UCLASS(Abstract)
class REMEMBERINGWALLACE_API AWeaponItemActor : public AItemInstanceActor
{
	GENERATED_BODY()

	// Allow access to private methods from the weapon item property
	friend UItemProperty_Weapon;

public:
	/**
	 * Constructor
	 */
	AWeaponItemActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Return the weapon item property in the item definition stored in the weapon item instance
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	const UItemProperty_Weapon* GetWeaponItemProperty() const;

	/**
	 * Return the equipment manager component which has this weapon equipped
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	UEquipmentManagerComponent* GetEquipmentManager() const;

	/**
	 * Return the weapon property data stored in the item instance
	 */
	const FItemPropertyData_Weapon* GetWeaponItemPropertyData() const;

	/**
	 * Return the weapon property data stored in the item instance
	 */
	FItemPropertyData_Weapon* GetWeaponItemPropertyData();

protected:
	/**
	 * Called when the weapon is equipped, before the blueprint event
	 *
	 * This is called after all other logics of equipping the weapon.
	 */
	virtual void Native_OnWeaponEquipped();

	/**
	 * Called when the weapon is to be unequipped, after the blueprint event
	 *
	 * This is called before any other logics are handled so that you still have access to related objects. However, be
	 * careful not to call any equip/unequip calls on the equipment manager component as this might create a loop.
	 */
	virtual void Native_OnWeaponAboutToBeUnequipped(bool bWillBeDestroyed);

	/**
	 * Called when the weapon is equipped
	 *
	 * This is called after all other logics of equipping the weapon.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon", DisplayName = "On Weapon Equipped")
	void BP_OnWeaponEquipped();

	/**
	 * Called when the weapon is unequipped
	 *
	 * This is called before any other logics are handled so that you still have access to related objects. However, be
	 * careful not to call any equip/unequip calls on the equipment manager component as this might create a loop.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon", DisplayName = "On Weapon About To Be Unequipped")
	void BP_OnWeaponAboutToBeUnequipped(bool bWillBeDestroyed);

private:
	/**
	 * Called by UItemProperty_Weapon when the weapon is equipped
	 */
	void OnWeaponEquipped();

	/**
	 * Called by UItemProperty_Weapon when the weapon is unequipped
	 */
	void OnWeaponAboutToBeUnequipped(bool bWillBeDestroyed);
};
