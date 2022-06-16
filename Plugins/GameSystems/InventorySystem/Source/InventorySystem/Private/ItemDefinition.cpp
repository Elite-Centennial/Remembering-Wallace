// Elite Centennial

#include "ItemDefinition.h"

#include "InventorySystem.h"
#include "ItemProperty.h"
#include "ItemProperty_Stack.h"

UItemDefinition::UItemDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const UItemProperty* UItemDefinition::GetProperty(
	const TSubclassOf<UItemProperty> PropertyClass) const
{
	if (PropertyClass)
	{
		for (const UItemProperty* Property : Properties)
		{
			if (Property && Property->IsA(PropertyClass))
			{
				return Property;
			}
		}
	}

	return nullptr;
}

int64 UItemDefinition::GetMaxStackCount() const
{
	const UItemProperty_Stack* StackProperty = GetProperty<UItemProperty_Stack>();
	return StackProperty ? StackProperty->GetMaxStackCount() : InventorySystem::GMaxStackCountDefault;
}
