// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WFC/WFCAdjacencyRules.h"
#include "WFCTile.generated.h"

UCLASS()
class REMEMBERINGWALLACE_API AWFCTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWFCTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UWFCAdjacencyRules* AdjacencyRules;
};
