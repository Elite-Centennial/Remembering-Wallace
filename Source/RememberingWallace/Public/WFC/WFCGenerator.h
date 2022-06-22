// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "WFC/WFCTile.h"
#include "WFCGenerator.generated.h"

/* FVirtualTile represents a tile using just 2 ints: an index in the master list
 * of tiles, and the number of 90 degree rotations. This spares us the overhead of
 * having to create an identical blueprint for every possible tile rotation, and
 * saves some memory in the process.
 */
struct FVirtualTile
{
	int TileIndex = -1;
	int Rotations = 0;
};

/* FValidTiles contains a list of Virtual Tiles, along with some precomputed properties
 * for the generator's helper functions
 */
struct FValidTiles
{
	TArray<FVirtualTile> tiles;
	int size = 0;
	FVirtualTile tile;
	TSubclassOf<AWFCTile> fromInput;
	bool hasPaths = false;
};

UCLASS()
class REMEMBERINGWALLACE_API AWFCGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWFCGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "WFC Generator")
	void CalculateBoundingBox();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, Category = "WFC Generator")
	int Width = 5;
	UPROPERTY(EditAnywhere, Category = "WFC Generator")
	int Height = 5;

	UPROPERTY(EditAnywhere, Category = "WFC Generator")
	FVector CellSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WFC Generator")
	TArray<TSubclassOf<AWFCTile>> Tiles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WFC Generator")
	TArray<TSubclassOf<AWFCTile>> InputArray;

	UPROPERTY(EditAnywhere, Category = "WFC Generator")
	int EdgeLabel = 0;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Instanced)
	UBoxComponent* BoundingBox;

private:
	TArray<FVirtualTile> GetValidTiles(TArray<int> edges);
	void CollapseCell(int x, int y);
	bool FindBestCell(int& x, int& y);

	void InitializeTiles();
	void WaveFunction();
	void SpawnTiles();

	TArray<FValidTiles> _TileStates;
	int forceSpawn = 1; // forces the algorithm to spawn at least this many tiles
	// otherwise, the mechanism for stopping when we've closed off the dungeon can prevent us from spawning a single tile
};
