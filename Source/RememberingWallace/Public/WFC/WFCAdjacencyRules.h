// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WFCAdjacencyRules.generated.h"


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

	UPROPERTY(EditAnywhere, Category = "Adjacency Rules")
	int Edges[4];

	UPROPERTY(EditAnywhere, Category = "Adjacency Rules")
	bool bRotatable = true;
};
