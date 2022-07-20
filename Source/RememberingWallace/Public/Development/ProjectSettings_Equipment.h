// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "Items/WeaponState.h"

#include "ProjectSettings_Equipment.generated.h"

class UAnimInstance;
class UAnimMontage;

/**
 * Configurable values for weapons depending on the type
 */
USTRUCT(BlueprintType)
struct REMEMBERINGWALLACE_API FWeaponDefaultSettings
{
	GENERATED_BODY()

	/**
	 * Names of sockets the weapon actor should be attached to
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponActorSocketNames SocketNames;

	/**
	 * Animation montage to use when drawing out the weapon
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> DrawMontage;

	/**
	 * Animation montage to use when sheathing the weapon
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> SheatheMontage;

	/**
	 * Animation blueprint class to link/unlink when the weapon is drawn/sheathed
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UAnimInstance> AnimClassToLink;
};

/**
 * Settings related to equipment (weapons and armors)
 */
UCLASS(Config = "Game", DefaultConfig, Const, meta = (DisplayName = "Equipment"))
class REMEMBERINGWALLACE_API UProjectSettings_Equipment final : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	/**
	 * Helper method to retrieve the default instance
	 */
	static const UProjectSettings_Equipment* Get() { return GetDefault<UProjectSettings_Equipment>(); }

	/**
	 * Constructor
	 */
	UProjectSettings_Equipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * A map of default settings for each weapon type
	 *
	 * Individual weapon items can override specific fields by assigning appropriate values in their weapon item
	 * properties in their definition files.
	 */
	UPROPERTY(GlobalConfig, EditDefaultsOnly, Category = "Weapon", DisplayName = "Default Settings Per Weapon Type",
		meta = (Categories = "Weapon.Type"))
	TMap<FGameplayTag, FWeaponDefaultSettings> WeaponDefaultSettings;
};

// TODO: Data validation?
