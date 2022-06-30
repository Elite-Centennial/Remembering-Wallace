// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "GameplayAbilityBase.generated.h"

/**
 * The base class of gameplay abilities used in this project
 */
UCLASS(HideCategories = "Input")
class REMEMBERINGWALLACE_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UGameplayAbilityBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Whether this ability can be automatically activated from player input
	 */
	bool ShouldActivateOnInput() const { return bActivateOnInput; }

protected:
	/**
	 * Whether this ability can be automatically activated from player input if bound to an input action
	 *
	 * Input pressed/released events are still sent even if this is set to false.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Triggers")
	bool bActivateOnInput;
};
