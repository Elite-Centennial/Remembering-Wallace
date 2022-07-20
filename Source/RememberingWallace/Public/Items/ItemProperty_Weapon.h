// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemProperty.h"
#include "WeaponState.h"
#include "AbilitySystem/AbilitySet.h"

#include "ItemProperty_Weapon.generated.h"

// Utility macro for getting weapon property from an item definition or an item instance
#define GetWeaponProperty() GetProperty<UItemProperty_Weapon>()

// Utility macro for getting weapon property data from an item instance
#define GetWeaponPropertyData() GetPropertyData<UItemProperty_Weapon>()

class AWeaponItemActor;
class UAnimInstance;
class UAnimMontage;
class UEquipmentManagerComponent;
class USceneComponent;
class USkeleton;
class UWallaceAbilitySystemComponent;
struct FItemPropertyData_Weapon;
struct FWeaponEquipParameters;

/**
 * Item definition property for weapon items
 */
UCLASS()
class REMEMBERINGWALLACE_API UItemProperty_Weapon : public UItemProperty
{
	GENERATED_BODY()

public:
	/**
	 * The data type for this property class
	 */
	typedef FItemPropertyData_Weapon FDataType;

	/**
	 * Constructor
	 */
	UItemProperty_Weapon();

	/**
	 * Return the type of this weapon
	 */
	const FGameplayTag& GetWeaponType() const { return WeaponType; }

	/**
	 * Return the trait tags of this weapon
	 */
	const FGameplayTagContainer& GetWeaponTraits() const { return WeaponTraits; }

	/**
	 * Return the ability sets to be granted when equipped
	 */
	const TArray<TObjectPtr<UAbilitySet>>& GetAbilitySets() const { return AbilitySets; }

	/**
	 * Return the actor class to spawn in world
	 */
	TSubclassOf<AWeaponItemActor> GetActorToSpawn() const { return ActorToSpawn; }

	/**
	 * Return the name of the socket the actor should be attached to
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Actor")
	FName GetActorPositionSocketName(EWeaponState Position) const;

	/**
	 * Return the animation montage to use when drawing out this weapon
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Animation")
	UAnimMontage* GetDrawMontage() const;

	/**
	 * Return the animation montage to use when sheathing this weapon
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Animation")
	UAnimMontage* GetSheatheMontage() const;

	/**
	 * Return the animation blueprint class to link when the weapon is drawn out
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Animation")
	TSubclassOf<UAnimInstance> GetAnimClassToLink() const;

	/**
	 * Apply the gameplay-mechanics-wise effects for equipping this weapon
	 *
	 * This does NOT set the equipped weapon value on the equipment manager component. Rather, this method is called
	 * from the equipment manager component when a new weapon is being equipped.
	 */
	void Equip(UItemInstance* WeaponInstance, UEquipmentManagerComponent* EquipmentManager) const;

	/**
	 * Apply the gameplay-mechanics-wise effects for unequipping this weapon
	 */
	void Unequip(UItemInstance* WeaponInstance, bool bDestroyActor = true) const;

	// BEGIN UItemProperty interface
	virtual FItemPropertyData* CreateNewDataInstance() const override;
	// END UItemProperty interface

protected:
	/**
	 * The type of this weapon (must start with "Weapon.Type")
	 *
	 * This is used to figure out several things related to the position of the actor and what animations to play.
	 * Typically, we'll want each weapon type to be only associated with one skeleton or a set of skeletons that are
	 * compatible to each other. Otherwise, playing animation montages and linking/unlinking anim BP classes won't work.
	 * To see what things are affected by this value, take a look at the equipment section in the project settings.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (Categories = "Weapon.Type"))
	FGameplayTag WeaponType;

	/**
	 * Tags that represent the traits of this weapon (must start with "Weapon", e.g. "Weapon.Shape.Curved")
	 *
	 * Currently NOT functional.
	 * TODO: apply weapon traits to owner when equipped
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (Categories = "Weapon"))
	FGameplayTagContainer WeaponTraits;

	/**
	 * Ability sets to grant when this weapon is equipped
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TArray<TObjectPtr<UAbilitySet>> AbilitySets;

	/**
	 * The actor class to use to spawn when the weapon is equipped
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeaponItemActor> ActorToSpawn;

	/**
	 * Names of sockets the weapon actor should be attached to; overrides the default in project settings
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Overrides", AdvancedDisplay, DisplayName = "Actor Socket Names")
	FWeaponActorSocketNames ActorSocketNamesOverride;

	/**
	 * Animation montage to use when drawing out this weapon; overrides the default in project settings
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Overrides", AdvancedDisplay, DisplayName = "Draw Montage")
	TObjectPtr<UAnimMontage> DrawMontageOverride;

	/**
	 * Animation montage to use when sheathing this weapon; overrides the default in project settings
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Overrides", AdvancedDisplay, DisplayName = "Sheathe Montage")
	TObjectPtr<UAnimMontage> SheatheMontageOverride;

	/**
	 * Animation blueprint class to link when this weapon is drawn out; overrides the default in project settings
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Overrides", AdvancedDisplay, DisplayName = "Anim BP To Link")
	TSubclassOf<UAnimInstance> AnimClassToLinkOverride;

	// TODO: data validation
};

/**
 * Data to store for each weapon instance
 */
USTRUCT(BlueprintType)
struct REMEMBERINGWALLACE_API FItemPropertyData_Weapon : public FItemPropertyData
{
	GENERATED_BODY()

	/**
	 * A weak pointer to the equipment manager component that this weapon is equipped to
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	TWeakObjectPtr<UEquipmentManagerComponent> EquipmentManager;

	/**
	 * A weak pointer to the spawned actor when the weapon is equipped
	 *
	 * A weak pointer is used to prevent blocking of garbage collection of the actor instance.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TWeakObjectPtr<AWeaponItemActor> SpawnedActor;

	/**
	 * Handles for the granted ability sets when equipped
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TArray<FAbilitySetGrantedHandles> GrantedHandles;

	/**
	 * Take back all granted ability sets and clean up the array
	 */
	void CleanUpGrantedHandles();

	// BEGIN FItemPropertyData interface
	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); }
	// END FItemPropertyData interface
};
