// Elite Centennial


#include "WFC/WFCAdjacencyRules.h"

// Sets default values for this component's properties
UWFCAdjacencyRules::UWFCAdjacencyRules()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UWFCAdjacencyRules::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWFCAdjacencyRules::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UWFCAdjacencyRules::IsCompatible(FTileEdges edges, int rotations)
{
	// an incompatible edge is one that isn't a wildcard AND has a non-matching id
	if (edges.Left != -1 && edges.Left != Edges.GetEdge(rotations % 4))
	{
		return false;
	}
	if (edges.Bottom != -1 && edges.Bottom != Edges.GetEdge((1 + rotations) % 4))
	{
		return false;
	}
	if (edges.Right != -1 && edges.Right != Edges.GetEdge((2 + rotations) % 4))
	{
		return false;
	}
	if (edges.Top != -1 && edges.Top != Edges.GetEdge((3 + rotations) % 4))
	{
		return false;
	}
	// if no edges fail, then this tile is compatible
	return true;
}

int UWFCAdjacencyRules::GetEdge(int edge, int rotations)
{
	if (bRotatable)
	{
		return Edges.GetEdge((edge + rotations) % 4);
	}

	return Edges.GetEdge(edge % 4);
}
