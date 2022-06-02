// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerController.h"

#include "WallacePlayerController.generated.h"

class UWallaceAbilitySystemComponent;

/**
 * Customized player controller class for this project
 */
UCLASS()
class REMEMBERINGWALLACE_API AWallacePlayerController : public APlayerController, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AWallacePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Return the associated ASC from the player state as our customized class
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AbilitySystem")
	UWallaceAbilitySystemComponent* GetWallaceAbilitySystemComponent() const;

	// BEGIN IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// END IAbilitySystemInterface interface
};
