// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"

#include "WallacePlayerState.generated.h"

class UWallaceAbilitySystemComponent;

/**
 * The player state class that is used in this project
 */
UCLASS()
class REMEMBERINGWALLACE_API AWallacePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	/**
	 * The name of the property holding the ASC
	 */
	static const FName AbilitySystemComponentName;

	/**
	 * Constructor
	 */
	AWallacePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Return the ASC as the customized type for this project
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AbilitySystem")
	UWallaceAbilitySystemComponent* GetWallaceAbilitySystemComponent() const { return AbilitySystemComponent; }

	// BEGIN IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// END IAbilitySystemInterface interface

protected:
	/**
	 * The ASC for the player
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UWallaceAbilitySystemComponent> AbilitySystemComponent;
};
