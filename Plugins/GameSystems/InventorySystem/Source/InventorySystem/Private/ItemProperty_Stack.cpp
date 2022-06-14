// Elite Centennial

#include "ItemProperty_Stack.h"

#include "InventorySystem.h"
#include "ItemInstance.h"

UItemProperty_Stack::UItemProperty_Stack()
{
	MaxStackCount = InventorySystem::GMaxStackCountDefault;
	bCanStackDefaultInstance = false;
}

bool UItemProperty_Stack::CanStack(const UItemInstance* Instance1, const UItemInstance* Instance2) const
{
	// TODO: Do checks for CanStack method

	// All checks have passed
	return true;
}

bool UItemProperty_Stack::CanStackDefault(const UItemInstance* Instance) const
{
	return bCanStackDefaultInstance;
}

FItemPropertyData* UItemProperty_Stack::CreateNewDataInstance() const
{
	FItemPropertyData_Stack* Data = new FItemPropertyData_Stack();
	Data->MaxStackCount = MaxStackCount;
	return Data;
}
