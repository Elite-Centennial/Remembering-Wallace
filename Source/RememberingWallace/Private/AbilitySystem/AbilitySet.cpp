// Elite Centennial

#include "AbilitySystem/AbilitySet.h"

#include "RememberingWallace.h"
#include "AbilitySystem/AbilityInputID.h"
#include "AbilitySystem/WallaceAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/AttributeSetBase.h"

void FAbilitySetGrantedHandles::TakeBack()
{
	UWallaceAbilitySystemComponent* AbilitySystemComponentPtr = AbilitySystemComponent.Get();

	// If the ASC is invalid, clear all data and exit early
	if (!AbilitySystemComponentPtr)
	{
		Clear();
		return;
	}

	// Remove the GAs, GEs, and attribute sets
	// The GEs and possibly GEs from GAs might reference attributes in the attribute sets. Therefore, to be safe, we
	// remove those in the order of GAs -> GEs -> attribute sets.

	// Remove gameplay abilities
	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystemComponentPtr->ClearAbility(Handle);
		}
	}

	// Remove gameplay effects
	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystemComponentPtr->RemoveActiveGameplayEffect(Handle);
		}
	}

	// Remove attribute sets
	for (const TWeakObjectPtr<UAttributeSetBase>& AttributeSetPtr : AttributeSets)
	{
		if (UAttributeSetBase* AttributeSet = AttributeSetPtr.Get())
		{
			AbilitySystemComponentPtr->GetSpawnedAttributes_Mutable().Remove(AttributeSet);
		}
	}

	// Clean up data
	Clear();
}

void FAbilitySetGrantedHandles::AddAttributeSet(UAttributeSetBase* AttributeSet)
{
	AttributeSets.Add(AttributeSet);
}

void FAbilitySetGrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FAbilitySetGrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FAbilitySetGrantedHandles::Clear()
{
	AttributeSets.Reset();
	GameplayEffectHandles.Reset();
	AbilitySpecHandles.Reset();
	AbilitySystemComponent.Reset();
}

// TODO: Add more options to UAbilitySet::Grant so that we can specify detailed context of what is granting this set
//       E.g., equipping a weapon or an armor can apply gameplay effects, and we can specify the effect causer in the
//       gameplay effect context when applying the effects.
bool UAbilitySet::Grant(
	UWallaceAbilitySystemComponent* AbilitySystemComponent,
	FAbilitySetGrantedHandles* OutHandles) const
{
	check(AbilitySystemComponent);

	// Check if the handle is for the same ASC if it is provided and has a valid ASC
	if (OutHandles)
	{
		if (const UWallaceAbilitySystemComponent* HandleASC = OutHandles->GetAbilitySystemComponent())
		{
			// The saved ASC is valid; check if it equals the target ASC
			if (HandleASC != AbilitySystemComponent)
			{
				// Different ASCs; abort
				return false;
			}
		}
		else
		{
			// The saved ASC is invalid; clear all saved handles before proceeding
			OutHandles->Clear();
		}
	}

	// Grant the GAs, GEs, and attribute sets to the ASC
	// The attribute sets must be added before the GEs and possibly GAs because the GEs might reference attributes that
	// are contained in the attribute sets. Thus, the order goes as attribute sets -> GEs -> GAs.

	// Add attribute sets
	// Index is needed for the error message.
	for (int32 Index = 0; Index < AttributeSets.Num(); ++Index)
	{
		const FAbilitySet_AttributeSet& SetToAdd = AttributeSets[Index];

		if (!IsValid(SetToAdd.AttributeSet))
		{
			UE_LOG(LogWallace, Error, TEXT("AttributeSets[%d] on ability set [%s] is not valid."),
				Index, *GetNameSafe(this));
			continue;
		}

		UAttributeSetBase* NewSet = NewObject<UAttributeSetBase>(
			AbilitySystemComponent->GetOwner(), SetToAdd.AttributeSet, SetToAdd.AttributeSetName);
		AbilitySystemComponent->AddAttributeSetSubobject(NewSet);

		// Add to the handles set if provided
		if (OutHandles)
		{
			OutHandles->AddAttributeSet(NewSet);
		}
	}

	// Apply gameplay effects
	// Index is needed for the error message.
	for (int32 Index = 0; Index < GameplayEffects.Num(); ++Index)
	{
		const FAbilitySet_GameplayEffect& EffectToApply = GameplayEffects[Index];

		if (!IsValid(EffectToApply.GameplayEffect))
		{
			UE_LOG(LogWallace, Error, TEXT("GameplayEffects[%d] on ability set [%s] is not valid."),
				Index, *GetNameSafe(this));
			continue;
		}

		const FActiveGameplayEffectHandle Handle = AbilitySystemComponent->BP_ApplyGameplayEffectToSelf(
			EffectToApply.GameplayEffect, EffectToApply.EffectLevel, AbilitySystemComponent->MakeEffectContext());

		// Add to the handles set if provided
		if (OutHandles)
		{
			OutHandles->AddGameplayEffectHandle(Handle);
		}
	}

	// Grant gameplay abilities
	// Index is needed for the error message.
	for (int32 Index = 0; Index < GameplayAbilities.Num(); ++Index)
	{
		const FAbilitySet_GameplayAbility& AbilityToGrant = GameplayAbilities[Index];

		if (!IsValid(AbilityToGrant.GameplayAbility))
		{
			UE_LOG(LogWallace, Error, TEXT("GameplayAbilities[%d] on ability set [%s] is not valid."),
				Index, *GetNameSafe(this));
			continue;
		}

		// Input ID should be set to the default value of -1 if the ability is not bound to any input
		const int32 InputID = AbilityToGrant.InputID == EAbilityInputID::None
			? INDEX_NONE
			: static_cast<uint8>(AbilityToGrant.InputID);

		const FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->K2_GiveAbility(
			AbilityToGrant.GameplayAbility, AbilityToGrant.AbilityLevel, InputID);

		// Add to the handles set if provided
		if (OutHandles)
		{
			OutHandles->AddAbilitySpecHandle(Handle);
		}
	}

	return true;
}

void UAbilitySet::BP_Grant(
	UWallaceAbilitySystemComponent* AbilitySystemComponent,
	FAbilitySetGrantedHandles& OutHandles) const
{
	Grant(AbilitySystemComponent, &OutHandles);
}
