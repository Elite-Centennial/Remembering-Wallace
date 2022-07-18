// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"

#include "DeveloperSettings_Characters.generated.h"

/**
 * Developer settings related to the characters
 */
UCLASS(Config = "Game", meta = (DisplayName = "Characters"))
class REMEMBERINGWALLACE_API UDeveloperSettings_Characters final : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UDeveloperSettings_Characters(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * The maximum distance for tracing for ground in the character movement component
	 *
	 * This is used for playing a smooth landing animation. This value should be larger than the maximum height of the
	 * character in the landing animation.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float GroundTraceDistance;
};
