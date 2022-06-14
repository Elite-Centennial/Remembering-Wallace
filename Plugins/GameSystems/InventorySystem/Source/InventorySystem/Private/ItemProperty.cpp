// Elite Centennial

#include "ItemProperty.h"

UItemProperty::UItemProperty(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UItemProperty::OnInstanceCreated_Implementation(UItemInstance* Instance) const
{
}

FItemPropertyData* UItemProperty::CreateNewDataInstance() const
{
	return nullptr;
}
