// Elite Centennial

#include "Actors/ItemInstanceActor.h"

#include "InventorySystemLog.h"
#include "ItemDefinition.h"
#include "ItemInstance.h"

AItemInstanceActor::AItemInstanceActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const UItemDefinition* AItemInstanceActor::GetItemDefinition() const
{
	return ItemInstance ? ItemInstance->GetDefinition() : nullptr;
}

void AItemInstanceActor::SetItemInstance(UItemInstance* InItemInstance)
{
	// Prevent calling the callbacks if there is no change
	if (ItemInstance.Get() == InItemInstance)
	{
		UE_LOG(LogInventory, Warning,
			TEXT("SetItemInstance called with same item instance [%s] of item [%s] for AItemInstanceActor [%s]"),
			*GetNameSafe(InItemInstance),
			*GetNameSafe(InItemInstance ? InItemInstance->GetDefinition() : nullptr),
			*GetName());
		return;
	}

	// Set the item instance to the new one
	UItemInstance* OldInstance = ItemInstance;
	ItemInstance = InItemInstance;

	// Invoke callbacks
	OnItemInstanceSet(InItemInstance, OldInstance);
	BP_OnItemInstanceSet(InItemInstance, OldInstance);
}
