// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"

#include "WeaponState.generated.h"

/**
 * The "mode" or "stance" regarding the weapon; sheathed or drawn
 */
UENUM(BlueprintType, Category = "Weapon|State")
enum class EWeaponState : uint8
{
	// The weapon is sheathed, meaning the unit is relaxed and not ready for combat
	Sheathed,
	// The weapon is drawn, meaning the unit is ready for engaging in combat
	Drawn,
};

/**
 * Names of sockets that weapon item actors can be attached to
 */
USTRUCT(BlueprintType)
struct REMEMBERINGWALLACE_API FWeaponActorSocketNames
{
	GENERATED_BODY()

	/**
	 * Name of socket to attach the weapon actor to when sheathed
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Sheathed;

	/**
	 * Name of socket to attach the weapon actor to when drawn
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Drawn;

	/**
	 * Return the name of the socket for the given weapon actor position
	 */
	FName GetSocketNameForPosition(EWeaponState Position) const;
};

/**
 * Blueprint function library for utility methods related to weapon state
 */
UCLASS(meta = (BlueprintThreadSafe))
class UWeaponStateLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Actor")
	static FName GetWeaponActorSocketName(const FWeaponActorSocketNames& SocketNames, EWeaponState Position);

	/**
	 * Returns the primary gameplay tag that represents the given weapon state
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|State")
	static FGameplayTag GetGameplayTagForWeaponState(EWeaponState WeaponState);

	/**
	 * Returns a gameplay tag container with gameplay tags corresponding to the given weapon state
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|State")
	static const FGameplayTagContainer& GetGameplayTagsForWeaponState(EWeaponState WeaponState);

	/**
	 * Returns the appropriate weapon state corresponding to the given gameplay tag
	 *
	 * The tag is considered invalid if it is not a recognized value. It must start with "Weapon.State". In case of an
	 * invalid tag, the returned value for the weapon state is "Sheathed". Currently, Drawing and Drawn are considered
	 * Drawn, whereas Sheathing and Sheathed are considered Sheathed.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|State", meta = (AutoCreateRefTerm = "Tag"))
	static EWeaponState GetWeaponStateFromGameplayTag(const FGameplayTag& Tag, bool& bValid);

	/**
	 * Returns the appropriate weapon state from the given gameplay tag container
	 *
	 * If no weapon state tags are found, the state is considered Sheathed.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|State", meta = (AutoCreateRefTerm = "Container"))
	static EWeaponState GetWeaponStateFromTagContainer(const FGameplayTagContainer& Container);

	/**
	 * Returns the appropriate weapon state from the given object that owns gameplay tags
	 *
	 * If no weapon state tags are found, the state is considered Sheathed.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|State")
	static EWeaponState GetWeaponStateFromTagOwner(TScriptInterface<IGameplayTagAssetInterface> TagOwner);
};
