// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "WallaceAbilitySystemComponent.generated.h"

/**
 * The ASC class that is used in this project
 */
UCLASS()
class REMEMBERINGWALLACE_API UWallaceAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UWallaceAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// BEGIN UAbilitySystemComponent interface
	virtual void BindToInputComponent(UInputComponent* InputComponent) override;
	virtual void AbilityLocalInputPressed(int32 InputID) override;
	virtual void AbilityLocalInputReleased(int32 InputID) override;
	// END UAbilitySystemComponent interface
};
