// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WFCAdjacencyRules.h"
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

	bool IsCompatible(const TArray<int> edges, int rotations);
	int GetEdge(int edge, int rotations);

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
		UWFCAdjacencyRules* AdjacencyRules;
};
