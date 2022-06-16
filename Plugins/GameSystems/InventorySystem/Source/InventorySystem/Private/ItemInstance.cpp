// Elite Centennial

#include "ItemInstance.h"

#include "InventorySystem.h"
#include "ItemDefinition.h"
#include "ItemProperty.h"
#include "ItemProperty_Stack.h"

UItemInstance* UItemInstance::CreateFromDefinition(const UItemDefinition* Definition, UObject* Outer)
{
	check(Definition);
	UItemInstance* Instance = NewObject<UItemInstance>(Outer);
	Instance->InitializeFromDefinition(Definition);
	return Instance;
}

UItemInstance* UItemInstance::CreateFromInstanceInit(const FItemInstanceInit& InstanceInit, UObject* Outer)
{
	check(InstanceInit.Definition.Get());
	UItemInstance* Instance = NewObject<UItemInstance>(Outer);
	Instance->InitializeFromInstanceInit(InstanceInit);
	return Instance;
}

const FItemPropertyData* UItemInstance::GetPropertyData(
	const TSubclassOf<UItemProperty> PropertyClass) const
{
	const TUniquePtr<FItemPropertyData>* DataPtr = InstanceData.Find(PropertyClass->GetFName());
	return DataPtr ? DataPtr->Get() : nullptr;
}

FItemPropertyData* UItemInstance::GetPropertyData(const TSubclassOf<UItemProperty> PropertyClass)
{
	const TUniquePtr<FItemPropertyData>* DataPtr = InstanceData.Find(PropertyClass->GetFName());
	return DataPtr ? DataPtr->Get() : nullptr;
}

int64 UItemInstance::GetMaxStackCount() const
{
	const FItemPropertyData_Stack* StackData = GetPropertyData<UItemProperty_Stack>();
	return StackData ? StackData->MaxStackCount : InventorySystem::GMaxStackCountDefault;
}

bool UItemInstance::CanStackWith(const UItemInstance* Other) const
{
	check(Other);

	// Every item instance can stack with itself
	if (this == Other)
	{
		return true;
	}

	// Only same type of items can be stacked
	if (Definition->GetIdentifierTag() != Other->Definition->GetIdentifierTag())
	{
		return false;
	}

	// Tell the property to figure out if two instances can be stacked
	if (const UItemProperty_Stack* StackProperty = Definition->GetProperty<UItemProperty_Stack>())
	{
		return StackProperty->CanStack(this, Other);
	}

	// Fallback
	return false;
}

bool UItemInstance::CanStackDefinition(const UItemDefinition* OtherDefinition) const
{
	check(OtherDefinition);

	// Only same type of items can be stacked
	if (Definition->GetIdentifierTag() != OtherDefinition->GetIdentifierTag())
	{
		return false;
	}

	// Tell the property to figure out if it can stack with a default instance
	if (const UItemProperty_Stack* StackProperty = Definition->GetProperty<UItemProperty_Stack>())
	{
		return StackProperty->CanStackDefault(this);
	}

	// Fallback
	return false;
}

void UItemInstance::InitializeFromDefinition(const UItemDefinition* InDefinition)
{
	// Assign item definition
	Definition = const_cast<UItemDefinition*>(InDefinition);

	// Populate the instance data from the properties
	for (const UItemProperty* Property : InDefinition->GetAllProperties())
	{
		if (Property)
		{
			InstanceData.Add(
				Property->GetClass()->GetFName(),
				TUniquePtr<FItemPropertyData>(Property->CreateNewDataInstance()));
		}
	}

	// Call OnInstanceCreated on each property
	for (const UItemProperty* Property : InDefinition->GetAllProperties())
	{
		if (Property)
		{
			Property->OnInstanceCreated(this);
		}
	}
}

void UItemInstance::InitializeFromInstanceInit(const FItemInstanceInit& InstanceInit)
{
	InitializeFromDefinition(InstanceInit.Definition);
}
