// Elite Centennial

#include "InventorySystemBlueprintLibrary.h"

#include "InventorySystem.h"

int64 UInventorySystemBlueprintLibrary::GetDefaultMaxStackCount()
{
	return InventorySystem::GMaxStackCountDefault;
}

bool UInventorySystemBlueprintLibrary::IsInventorySlotValid(const FInventorySlot& Slot)
{
	return Slot.IsValid();
}
