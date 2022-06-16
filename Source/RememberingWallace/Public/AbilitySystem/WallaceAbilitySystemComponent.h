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
	UWallaceAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
