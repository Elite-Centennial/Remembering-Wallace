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
	/**
	 * Get the default value for max stack count
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Stack")
	static int64 GetDefaultMaxStackCount();

	/**
	 * Whether the given inventory slot has valid data
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	static bool IsInventorySlotValid(const FInventorySlot& Slot);
};
