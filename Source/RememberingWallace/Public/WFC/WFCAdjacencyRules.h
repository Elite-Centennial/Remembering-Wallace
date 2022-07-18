// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WFCAdjacencyRules.generated.h"

USTRUCT(BlueprintType)
struct FTileEdges
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	int Left = 0;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	int Bottom = 0;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	int Right = 0;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	int Top = 0;

	// helper for some functions that still use indices to read an edge
	int GetEdge(int id)
	{
		if (id == 3)
		{
			return Top;
		}
		if (id == 2)
		{
			return Right;
		}
		if (id == 1)
		{
			return Bottom;
		}
		return Left;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REMEMBERINGWALLACE_API UWFCAdjacencyRules : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWFCAdjacencyRules();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsCompatible(FTileEdges edges, int rotations);
	int GetEdge(int edge, int rotations);

	UPROPERTY(EditAnywhere, Category = "Adjacency Rules")
	FTileEdges Edges;

	UPROPERTY(EditAnywhere, Category = "Adjacency Rules")
	bool bRotatable = true;
};
