// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "InventorySystemBlueprintLibrary.generated.h"

/**
 * Blueprint functions for the inventory system
 */
UCLASS()
class INVENTORYSYSTEM_API UInventorySystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	static int64 GetStackCount(const FInventorySlot& InventorySlot);
};
