// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "InventorySystemInterface.generated.h"

class UInventoryComponent;

/**
 * Interface for returning an inventory component
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UInventorySystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for returning an inventory component
 */
class INVENTORYSYSTEM_API IInventorySystemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	UInventoryComponent* GetInventoryComponent() const;
};
