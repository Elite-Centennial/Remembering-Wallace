// Elite Centennial


#include "WFC/WFCTile.h"

// Sets default values
AWFCTile::AWFCTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;

	AdjacencyRules = CreateDefaultSubobject<UWFCAdjacencyRules>(TEXT("Adjacency Rules"));
}

// Called when the game starts or when spawned
void AWFCTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWFCTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AWFCTile::IsCompatible(const TArray<int> edges, int rotations)
{
	for (int i = 0; i < 4; i++)
	{
		if (edges[i] != -1 && edges[i] != AdjacencyRules->Edges[(i + rotations) % 4])
		{
			return false;
		}
	}
	return true;
}

int AWFCTile::GetEdge(int edge, int rotations)
{
	if (AdjacencyRules->bRotatable)
	{
		return AdjacencyRules->Edges[(edge + rotations) % 4];
	}

	return AdjacencyRules->Edges[edge % 4];
}
