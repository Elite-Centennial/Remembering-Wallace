// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "EquipmentManagerComponent.generated.h"

enum class EWeaponState : uint8;
class AWeaponItemActor;
class UInventoryComponent;
class UItemInstance;
class UWallaceAbilitySystemComponent;

/**
 * Actor component that handles equipping weapons and armors
 */
UCLASS(ClassGroup = "Gameplay", meta = (BlueprintSpawnableComponent))
class REMEMBERINGWALLACE_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Returns the item instance of currently equipped weapon
	 */
	const UItemInstance* GetCurrentWeapon() const { return Weapon; }

	/**
	 * Whether there is a weapon currently equipped
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment|Weapon")
	bool HasWeaponEquipped() const { return !!Weapon; }

	/**
	 * The current state of the weapon, e.g. drawn or sheathed
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment|Weapon")
	EWeaponState GetCurrentWeaponState() const;

	/**
	 * Equip the given weapon and return the one previously held
	 *
	 * The previous weapon is null when no changes were made.
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Weapon")
	void EquipWeapon(
		bool& bChanged, UItemInstance*& PreviousWeapon, UItemInstance* NewWeapon, bool bDestroyActor = true);

	/**
	 * Equip the given weapon and return whether a change was made
	 *
	 * This overload is a utility wrapper to the other overload.
	 */
	bool EquipWeapon(UItemInstance* NewWeapon, bool bDestroyActor = true);

	/**
	 * Un-equip the currently held weapon and return it
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Weapon", meta = (ReturnDisplayName = "Previous Weapon"))
	UItemInstance* UnequipWeapon(bool bDestroyActor = true);

	/**
	 * Return the spawned actor for the current weapon
	 */
	AWeaponItemActor* GetCurrentWeaponActor() const;

	/**
	 * Return the spawned actor for the current weapon
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = "false", Category = "Equipment|Weapon",
		DisplayName = "Get Current Weapon Actor", meta = (ExpandBoolAsExecs = "bValid"))
	AWeaponItemActor* BP_GetCurrentWeaponActor(bool& bValid) const;

	/**
	 * Return the cached ASC of the owning actor
	 */
	UWallaceAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }

	/**
	 * Return the cached inventory component of the owning actor
	 */
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	/**
	 * Return the main skeletal mesh component of the owning actor, retrieved from the avatar actor of the ASC
	 */
	USkeletalMeshComponent* GetSkeletalMeshComponent() const;

	// BEGIN UActorComponent interface
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	// END UActorComponent interface

private:
	/**
	 * Item instance of currently equipped weapon
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess))
	TObjectPtr<UItemInstance> Weapon;

	/**
	 * Cached pointer to the ASC of the owning actor
	 */
	UPROPERTY(Transient)
	TObjectPtr<UWallaceAbilitySystemComponent> AbilitySystemComponent;

	/**
	 * Cached pointer to the inventory component of the owning actor
	 */
	UPROPERTY(Transient)
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
