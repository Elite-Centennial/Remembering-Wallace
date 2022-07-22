// Elite Centennial

#include "GameInstanceBase.h"

#include "RememberingWallace.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectGlobals.h"

UGameInstanceBase::UGameInstanceBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameInstanceBase::ChangeMap(const TSoftObjectPtr<UWorld> NewMap)
{
	DestinationMap = NewMap;
	OnChangeLevelRequested();
}

void UGameInstanceBase::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGameInstanceBase::HandleMapLoadFinished);
}

void UGameInstanceBase::Shutdown()
{
	RemoveLoadingScreenFromViewport();

	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);

	Super::Shutdown();
}

void UGameInstanceBase::StartLoadingMapAsset()
{
	LoadPackageAsync(
		DestinationMap.GetLongPackageName(),
		FLoadPackageAsyncDelegate::CreateUObject(this, &UGameInstanceBase::HandleAsyncPackageLoadFinished),
		0,
		PKG_ContainsMap);
}

UUserWidget* UGameInstanceBase::ShowLoadingScreen(const TSubclassOf<UUserWidget> LoadingScreenClass, const int32 ZOrder)
{
	if (bLoadingScreenShown)
	{
		return CurrentLoadingScreen.Get();
	}

	if (UUserWidget* Widget = UUserWidget::CreateWidgetInstance(*this, LoadingScreenClass, EName::None))
	{
		bLoadingScreenShown = true;
		CurrentLoadingScreen = Widget;
		GetGameViewportClient()->AddViewportWidgetContent(Widget->TakeWidget(), ZOrder);
		return Widget;
	}

	return nullptr;
}

void UGameInstanceBase::HideLoadingScreen()
{
	if (!bLoadingScreenShown)
	{
		return;
	}

	RemoveLoadingScreenFromViewport();
	bLoadingScreenShown = false;
}

void UGameInstanceBase::RemoveLoadingScreenFromViewport()
{
	// If the pointer to the widget is null, do nothing
	if (!CurrentLoadingScreen)
	{
		return;
	}

	// Get the underlying low-level widget, hopefully the same one created from TakeWidget method in ShowLoadingScreen
	const TSharedPtr<SWidget> Widget = CurrentLoadingScreen->GetCachedWidget();

	if (Widget.IsValid())
	{
		GetGameViewportClient()->RemoveViewportWidgetContent(Widget.ToSharedRef());
	}

	// Remove the reference to the widget so that it can be garbage collected
	CurrentLoadingScreen = nullptr;
}

void UGameInstanceBase::HandleAsyncPackageLoadFinished(
	const FName& PackageName, UPackage* LoadedPackage, const EAsyncLoadingResult::Type Result)
{
	if (Result != EAsyncLoadingResult::Succeeded)
	{
		// TODO: error case handling
		UE_LOG(LogWallace, Error, TEXT("Failed to async load package [%s]"), *PackageName.ToString());
		return;
	}

	const UWorld* LoadedWorld = UWorld::FindWorldInPackage(LoadedPackage);
	check(LoadedWorld && DestinationMap == LoadedWorld);
	LoadedMapPackage = LoadedPackage;
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
}

void UGameInstanceBase::HandleMapLoadFinished(UWorld* LoadedWorld)
{
	// Clean up the reference to the package if the level is loaded into the game
	if (LoadedMapPackage.Get())
	{
		if (LoadedWorld->GetName() == UWorld::FindWorldInPackage(LoadedMapPackage)->GetName())
		{
			OnMapTransitionFinished();
			LoadedMapPackage = nullptr;
			DestinationMap.Reset();
		}
	}
}
