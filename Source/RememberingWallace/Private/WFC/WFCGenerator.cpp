// Elite Centennial


#include "WFC/WFCGenerator.h"

// Sets default values
AWFCGenerator::AWFCGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounds"));
	BoundingBox->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void AWFCGenerator::BeginPlay()
{
	Super::BeginPlay();

	InitializeTiles();

	WaveFunction();

	SpawnTiles();

	// remove box collider. If for some reason we need it later, remove the following lines
	BoundingBox->DestroyComponent();
}

// Called every frame
void AWFCGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWFCGenerator::CalculateBoundingBox()
{
	BoundingBox->SetBoxExtent(FVector(CellSize.X * Width / 2, CellSize.Y * Height / 2, CellSize.Z / 2));
	BoundingBox->SetRelativeLocation(FVector((CellSize.X * Width / 2) - (CellSize.X / 2),
		(CellSize.Y * Height / 2) - (CellSize.Y / 2), CellSize.Z / 2));
}

void AWFCGenerator::InitializeTiles()
{
	// initialize tile states
	{
		FTileEdges wildcard = {-1, -1, -1, -1};
		TArray<FVirtualTile> indices = GetValidTiles(wildcard); // expensive, but builds an initial
		// array of virtual tile representations of the actual tile array
		int size = indices.Num();

		_TileStates.Init({ indices, size }, Width * Height);
	}

	// place input tiles
	for (int i = 0; i < InputArray.Num() && i < _TileStates.Num(); i++)
	{
		if (InputArray[i] != NULL)
		{
			_TileStates[i].fromInput = InputArray[i];
			_TileStates[i].tiles.Empty();
			_TileStates[i].size = 1;
		}
	}

	// one full pass through the grid to collapse cells to intial states
	for (int x = 0; x < Width; x++)
	{
		for (int y = 0; y < Height; y++)
		{
			CollapseCell(x, y);
		}
	}
}

TArray<FVirtualTile> AWFCGenerator::GetValidTiles(const FTileEdges& edges)
{
	TArray<FVirtualTile> validTiles = {};

	for (int i = 0; i < Tiles.Num(); i++)
	{
		AWFCTile* tileObject = Tiles[i].GetDefaultObject();
		
		if (tileObject->AdjacencyRules->IsCompatible(edges, 0))
		{
			validTiles.Add({ i, 0 });
		}

		if (tileObject->AdjacencyRules->bRotatable)
		{
			// we're rotating quads -> there's a bunch of magic 4's in this code and I can't be bothered to clean them up
			for (int j = 1; j < 4; j++)
			{
				if (tileObject->AdjacencyRules->IsCompatible(edges, j))
				{
					validTiles.Add({ i, j });
				}
			}
		}
	}

	return validTiles;
}

// "believe it or not this is the shortened version"
void AWFCGenerator::CollapseCell(int x, int y)
{
	FTileEdges edges = {-1, -1, -1, -1}; // populate with wildcards by default
	int index = (y * Width) + x; // precomputing because it comes up a lot in here

	if (_TileStates[index].tile.TileIndex != -1 || _TileStates[index].fromInput != NULL)
	{
		return;
	}

	// check left
	if (x - 1 < 0)
	{
		edges.Left = EdgeLabel;
	}
	else if (_TileStates[index - 1].size == 1)
	{
		// this got cluttery because of the input array behavior
		if (_TileStates[index - 1].fromInput != NULL)
		{
			edges.Left = _TileStates[index - 1].fromInput.GetDefaultObject()->AdjacencyRules->Edges.Right;
		}
		else
		{
			edges.Left = Tiles[_TileStates[index - 1].tile.TileIndex]
				.GetDefaultObject()->AdjacencyRules->GetEdge(2, _TileStates[index - 1].tile.Rotations);
		}
	}

	// check right
	if (x + 1 >= Width)
	{
		edges.Right = EdgeLabel;
	}
	else if (_TileStates[index + 1].size == 1)
	{
		if (_TileStates[index + 1].fromInput != NULL)
		{
			edges.Right = _TileStates[index + 1].fromInput.GetDefaultObject()->AdjacencyRules->Edges.Left;
		}
		else
		{
			edges.Right = Tiles[_TileStates[index + 1].tile.TileIndex]
				.GetDefaultObject()->AdjacencyRules->GetEdge(0, _TileStates[index + 1].tile.Rotations);
		}
	}

	// check above
	if (y - 1 < 0)
	{
		edges.Top = EdgeLabel;
	}
	else if (_TileStates[index - Width].size == 1)
	{
		if (_TileStates[index - Width].fromInput != NULL)
		{
			edges.Top = _TileStates[index - Width].fromInput.GetDefaultObject()->AdjacencyRules->Edges.Bottom;
		}
		else
		{
			edges.Top = Tiles[_TileStates[index - Width].tile.TileIndex]
				.GetDefaultObject()->AdjacencyRules->GetEdge(1, _TileStates[index - Width].tile.Rotations);
		}
	}

	// check below
	if (y + 1 >= Height)
	{
		edges.Bottom = EdgeLabel;
	}
	else if (_TileStates[index + Width].size == 1)
	{
		if (_TileStates[index + Width].fromInput != NULL)
		{
			edges.Bottom = _TileStates[index + Width].fromInput.GetDefaultObject()->AdjacencyRules->Edges.Top;
		}
		else
		{
			edges.Bottom = Tiles[_TileStates[index + Width].tile.TileIndex]
				.GetDefaultObject()->AdjacencyRules->GetEdge(3, _TileStates[index + Width].tile.Rotations);
		}
	}

	_TileStates[index].tiles = GetValidTiles(edges);
	_TileStates[index].size = _TileStates[index].tiles.Num();
	for (int i = 0; i < 4; i++)
	{
		// -1 and 0 are considered non-pathway edges
		if (edges.GetEdge(i) > 0)
		{
			_TileStates[index].hasPaths = true;
			break;
		}
	}
}

bool AWFCGenerator::FindBestCell(int& x, int& y)
{
	int smallest = Tiles.Num() * 4 + 1; // maximum possible number of tiles
	int smallestIndex = -1;

	for (int i = 0; i < _TileStates.Num(); i++)
	{
		// ignore cells that have already been collapsed
		if (_TileStates[i].tile.TileIndex != -1)
		{
			continue;
		}
		if (_TileStates[i].size < smallest && (forceSpawn > 0 || _TileStates[i].hasPaths))
		{
			smallestIndex = i;
			smallest = _TileStates[i].size;
		}
		// in case of a tie, we prioritize the first cell found, this causes a top-left corner bias but in
		// practice should not matter if the grid has at least one tile already inserted before generating
	}

	if (smallestIndex == -1)
	{
		// signal that we could not decide on a cell to collapse
		return false;
	}
	x = smallestIndex % Width;
	y = smallestIndex / Width;
	return true;
}

void AWFCGenerator::WaveFunction()
{
	// loop until all cells are collapsed (or until FindSmallestCell returns false)
	int x = 0;
	int y = 0;
	while (FindBestCell(x, y))
	{
		int index = x + (y * Width);
		if (_TileStates[index].tiles.Num() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("WFCGenerator Error: no suitable tile found for cell %d %d . Make sure your tileset is complete"), x, y);
			_TileStates[index].tile.TileIndex = 0;
			_TileStates[index].size = 1;
			_TileStates[index].tiles.Empty();
		}
		else
		{
			int chosenTile = FMath::RandRange(0, _TileStates[index].tiles.Num() - 1);
			_TileStates[index].tile = _TileStates[index].tiles[chosenTile];
			_TileStates[index].size = 1;
			_TileStates[index].tiles.Empty();
		}

		// collapse neighbors
		if (x > 0) CollapseCell(x - 1, y);
		if (x < Width - 1) CollapseCell(x + 1, y);
		if (y > 0) CollapseCell(x, y - 1);
		if (y < Height - 1) CollapseCell(x, y + 1);

		// misc
		forceSpawn--;
	}
}

void AWFCGenerator::SpawnTiles()
{
	FVector rootPosition = SceneComponent->GetComponentTransform().GetLocation();
	// these vectors are relative to the generator's rotation so we can effectively rotate the entire dungeon being generated
	FVector forward = SceneComponent->GetForwardVector() * CellSize.X; // "forward" refers to the positive x axis
	FVector right = SceneComponent->GetRightVector() * CellSize.Y; // "right" refers to the positive y axis
	UWorld* world = GetWorld();
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			int index = x + (y * Width);
			if (world != nullptr)
			{
				AWFCTile* spawnedActor = nullptr;
				if (_TileStates[index].fromInput != NULL)
				{
					spawnedActor = world->SpawnActor<AWFCTile>(_TileStates[index].fromInput->GetAuthoritativeClass());
					FRotator rotation = FRotator(0, SceneComponent->GetComponentTransform().GetRotation().Euler().Z, 0);
					FQuat quaternion = FQuat(rotation);
					//FVector position(rootPosition.X + (x * CellSize.X), rootPosition.Y + (y * CellSize.Y), rootPosition.Z);
					FVector position(rootPosition + (right * y) + (forward * x));
					spawnedActor->SceneComponent->SetWorldTransform(FTransform(position));
					spawnedActor->AddActorLocalRotation(quaternion, false, 0, ETeleportType::ResetPhysics);
					spawnedActor->SetActorLabel(*FString("Tile" + FString::FromInt(index)));
				}
				else if (_TileStates[index].tile.TileIndex != -1)
				{
					spawnedActor = world->SpawnActor<AWFCTile>(Tiles[_TileStates[index].tile.TileIndex]->GetAuthoritativeClass());
					FRotator rotation = FRotator(0,
						(_TileStates[index].tile.Rotations * 90) + SceneComponent->GetComponentTransform().GetRotation().Euler().Z, 0);
					FQuat quaternion = FQuat(rotation);
					//FVector position(rootPosition.X + (x * CellSize.X), rootPosition.Y + (y * CellSize.Y), rootPosition.Z);
					FVector position(rootPosition + (right * y) + (forward * x));
					spawnedActor->SceneComponent->SetWorldTransform(FTransform(position));
					spawnedActor->AddActorLocalRotation(quaternion, false, 0, ETeleportType::ResetPhysics);
					spawnedActor->SetActorLabel(*FString("Tile" + FString::FromInt(index)));

				}
			}
		}
	}
}