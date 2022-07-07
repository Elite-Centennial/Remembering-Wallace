// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"

#include "DeveloperSettings_Equipment.generated.h"

class UAnimInstance;
class USkeleton;

USTRUCT(BlueprintType)
struct REMEMBERINGWALLACE_API FAnimClassPerWeaponType
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, NoClear, meta = (Categories = "Weapon.Type"))
	TMap<FGameplayTag, TSoftClassPtr<UAnimInstance>> Map;
};

/**
 * Developer settings related to equipment (weapons and armors)
 */
UCLASS(Config = "Game", DefaultConfig, meta = (DisplayName = "Equipment"))
class REMEMBERINGWALLACE_API UDeveloperSettings_Equipment final : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UDeveloperSettings_Equipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * A map of animation blueprint classes that will be linked for each weapon type
	 *
	 * Individual weapon items can override this by assigning a different class to
	 * UItemProperty_Weapon::AnimClassToLinkOverrides field.
	 */
	UPROPERTY(GlobalConfig, EditDefaultsOnly, Category = "Weapon|Animation", NoClear, DisplayName = "Anim BP to Link")
	TMap<TSoftObjectPtr<USkeleton>, FAnimClassPerWeaponType> AnimClassToLinkPerWeaponType;
};
