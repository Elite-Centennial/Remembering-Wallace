// Elite Centennial

#include "GameplayEventHelper.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Abilities/GameplayAbilityTypes.h"

void RememberingWallace::GameplayEvent::SendEventsForAttrGEExecute(
	const FGameplayEffectModCallbackData* Data,
	UAbilitySystemComponent* TargetASC,
	const FGameplayTag& TagForSource,
	const FGameplayTag& TagForTarget,
	const float Magnitude,
	FGameplayEventData* OutEventData)
{
	// Prepare the place to store the data in case the pointer is null
	FGameplayEventData EventData;

	if (!OutEventData)
	{
		// The pointer is null, so point it to the place we prepared above
		OutEventData = &EventData;
	}

	// Prepare the place to store the ASC of the source
	UAbilitySystemComponent* InstigatorASC = nullptr;

	// Gather event data
	GatherEventDataForAttrGEExecute(Data, Magnitude, OutEventData, &InstigatorASC);

	if (InstigatorASC)
	{
		// Send the event to the source
		OutEventData->EventTag = TagForSource;
		InstigatorASC->HandleGameplayEvent(OutEventData->EventTag, OutEventData);
	}

	// Change the tag and send the event to the target, which is the owner of this instance
	OutEventData->EventTag = TagForTarget;
	TargetASC->HandleGameplayEvent(OutEventData->EventTag, OutEventData);
}

void RememberingWallace::GameplayEvent::GatherEventDataForAttrGEExecute(
	const FGameplayEffectModCallbackData* Data,
	const float Magnitude,
	FGameplayEventData* OutEventData,
	UAbilitySystemComponent** OutInstigatorASC)
{
	check(OutEventData);

	// Prepare the place to store the value in case the pointer is null
	UAbilitySystemComponent* InstigatorASC = nullptr;

	if (!OutInstigatorASC)
	{
		// The pointer is null, so point it to the place we prepared above
		OutInstigatorASC = &InstigatorASC;
	}

	// We're going to use these to populate the event data
	const FGameplayEffectContextHandle& Context = Data->EffectSpec.GetEffectContext();
	*OutInstigatorASC = Context.GetOriginalInstigatorAbilitySystemComponent();

	// Populate data into the event data struct
	OutEventData->Instigator = Context.GetOriginalInstigator();
	OutEventData->Target = Data->Target.GetAvatarActor() ? Data->Target.GetAvatarActor() : Data->Target.GetOwnerActor();
	OutEventData->ContextHandle = Context;
	OutEventData->EventMagnitude = Magnitude;
	Data->Target.GetOwnedGameplayTags(OutEventData->TargetTags);

	if (*OutInstigatorASC)
	{
		(*OutInstigatorASC)->GetOwnedGameplayTags(OutEventData->InstigatorTags);
	}
}
