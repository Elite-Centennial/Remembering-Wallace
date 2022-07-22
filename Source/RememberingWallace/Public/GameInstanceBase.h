// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "GameInstanceBase.generated.h"

/**
 * Base class for the game instance class for this project
 */
UCLASS()
class REMEMBERINGWALLACE_API UGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UGameInstanceBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Whether the loading screen is currently shown on screen
	 */
	bool IsLoadingScreenShown() const { return bLoadingScreenShown; }

	/**
	 * Start the process of unloading the previous map and loading the requested map
	 */
	UFUNCTION(BlueprintCallable, Category = "Map")
	void ChangeMap(TSoftObjectPtr<UWorld> NewMap);

	// BEGIN UGameInstance interface
	virtual void Init() override;
	virtual void Shutdown() override;
	// END UGameInstance interface

protected:
	/**
	 * The map used for transition between maps
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	TSoftObjectPtr<UWorld> TransitionMap;

	/**
	 * Cached soft pointer to the map to transition to
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Map")
	TSoftObjectPtr<UWorld> DestinationMap;

	/**
	 * Whether the loading screen is currently shown on screen
	 */
	UPROPERTY(BlueprintReadOnly, Category = "LoadingScreen")
	bool bLoadingScreenShown;

	/**
	 * Reference to the currently shown loading screen widget
	 */
	UPROPERTY(BlueprintReadOnly, Category = "LoadingScreen")
	TObjectPtr<UUserWidget> CurrentLoadingScreen;

	/**
	 * Blueprint event called when ChangeLevel is called
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Map")
	void OnChangeLevelRequested();

	/**
	 * Blueprint event called when the transition to the requested map is done
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Map")
	void OnMapTransitionFinished();

	/**
	 * Start asynchronously loading the requested map asset into the memory
	 */
	UFUNCTION(BlueprintCallable, Category = "Map")
	void StartLoadingMapAsset();

	/**
	 * Create a new loading screen and show it on screen
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "LoadingScreen",
		meta = (DeterminesOutputType = "LoadingScreenClass"))
	UUserWidget* ShowLoadingScreen(TSubclassOf<UUserWidget> LoadingScreenClass, int32 ZOrder = 0);

	/**
	 * Destroy the currently shown loading screen if it exists
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "LoadingScreen")
	void HideLoadingScreen();

private:
	// Remove the loading screen from the viewport and reset the reference to the widget
	void RemoveLoadingScreenFromViewport();

	// Handler method of the delegate for LoadPackageAsync
	void HandleAsyncPackageLoadFinished(
		const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result);

	// The loaded package for the map (prevents garbage collection)
	UPROPERTY()
	TObjectPtr<UPackage> LoadedMapPackage;

	// Handler method of FCoreUObjectDelegates::PostLoadMapWithWorld
	UFUNCTION()
	void HandleMapLoadFinished(UWorld* LoadedWorld);
};
