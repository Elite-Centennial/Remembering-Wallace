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
