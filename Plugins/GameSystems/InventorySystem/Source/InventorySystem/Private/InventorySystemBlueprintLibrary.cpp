// Elite Centennial

#include "InventorySystemBlueprintLibrary.h"

int64 UInventorySystemBlueprintLibrary::GetStackCount(const FInventorySlot& InventorySlot)
{
	return InventorySlot.StackCount;
}
