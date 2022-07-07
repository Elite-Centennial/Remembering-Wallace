// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemProperty.h"
#include "AbilitySystem/AbilitySet.h"

#include "ItemProperty_Weapon.generated.h"

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
	 * Return the actor class to spawn in world
	 */
	TSubclassOf<AWeaponItemActor> GetActorToSpawn() const { return ActorToSpawn; }

	/**
	 * Return the name of the socket the actor should be attached to when sheathed
	 */
	const FName& GetSheathedSocketName() const { return SheathedSocketName; }

	/**
	 * Return the name of the socket the actor should be attached to when drawn out
	 */
	const FName& GetDrawnSocketName() const { return DrawnSocketName; }

	/**
	 * Return the type of this weapon
	 */
	const FGameplayTag& GetWeaponType() const { return WeaponType; }

	/**
	 * Return the ability set to be granted when equipped
	 */
	const UAbilitySet* GetAbilitySet() const { return AbilitySet; }

	/**
	 * Return the animation montage to use when drawing out this weapon
	 */
	UAnimMontage* GetDrawMontage() const { return DrawMontage; }

	/**
	 * Return the animation montage to use when sheathing this weapon
	 */
	UAnimMontage* GetSheatheMontage() const { return SheatheMontage; }

	/**
	 * Return the animation blueprint class to link when the weapon is drawn out
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Animation")
	TSubclassOf<UAnimInstance> GetAnimClassToLink(const USkeleton* Skeleton) const;

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
	 * The actor class to use to spawn when the weapon is equipped
	 */
	// TODO: Move this to a new item property class for more general usage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor")
	TSubclassOf<AWeaponItemActor> ActorToSpawn;

	/**
	 * The name of the socket the weapon actor should be attached to when it is sheathed
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor")
	FName SheathedSocketName;

	/**
	 * The name of the socket the weapon actor should be attached to when it is drawn out
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor")
	FName DrawnSocketName;

	/**
	 * The type of this weapon
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (Categories = "Weapon.Type"))
	FGameplayTag WeaponType;

	/**
	 * Ability set to grant when this weapon is equipped
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UAbilitySet> AbilitySet;

	/**
	 * Animation montage to use when drawing out this weapon
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DrawMontage;

	/**
	 * Animation montage to use when sheathing this weapon
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> SheatheMontage;

	/**
	 * Animation blueprint class to link when this weapon is drawn out
	 *
	 * This overrides the settings in UDeveloperSettings_Equipment just for this weapon.
	 */
	// TODO: Change to a plain anim BP class? Or change the montages to a map?
	UPROPERTY(EditDefaultsOnly, Category = "Animation", AdvancedDisplay, NoClear)
	TMap<TObjectPtr<USkeleton>, TSubclassOf<UAnimInstance>> AnimClassToLinkOverrides;
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
	 * Handles for the granted ability set when equipped
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	FAbilitySetGrantedHandles GrantedHandles;

	/**
	 * Class of the animation blueprint that was linked when the weapon was drawn out
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> LinkedAnimClass;

	// BEGIN FItemPropertyData interface
	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); }
	// END FItemPropertyData interface
};
