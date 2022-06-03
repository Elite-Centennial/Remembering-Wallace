// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"

#include "AttributeSetBase.generated.h"

/**
 * Base class for all attribute sets
 */
UCLASS(Abstract)
class REMEMBERINGWALLACE_API UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeSetBase();
};
