// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "InventorySystemInterface.generated.h"

class UInventoryComponent;

UINTERFACE(BlueprintType)
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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UInventoryComponent* GetInventoryComponent() const;
};
