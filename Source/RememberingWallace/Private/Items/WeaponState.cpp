// Elite Centennial

#include "Items/WeaponState.h"

#include "RememberingWallaceTags.h"

FName FWeaponActorSocketNames::GetSocketNameForPosition(const EWeaponState Position) const
{
	switch (Position)
	{
	case EWeaponState::Sheathed:
		return Sheathed;
	case EWeaponState::Drawn:
		return Drawn;
	default:
		checkNoEntry();
		return EName::None;
	}
}

FName UWeaponStateLibrary::GetWeaponActorSocketName(
	const FWeaponActorSocketNames& SocketNames, const EWeaponState Position)
{
	return SocketNames.GetSocketNameForPosition(Position);
}

FGameplayTag UWeaponStateLibrary::GetGameplayTagForWeaponState(const EWeaponState WeaponState)
{
	switch (WeaponState)
	{
	case EWeaponState::Sheathed:
		return TAG(Weapon.State.Sheathed);
	case EWeaponState::Drawn:
		return TAG(Weapon.State.Drawn);
	default:
		return FGameplayTag();
	}
}

const FGameplayTagContainer& UWeaponStateLibrary::GetGameplayTagsForWeaponState(const EWeaponState WeaponState)
{
	static const TMap<EWeaponState, FGameplayTagContainer> Containers =
	{
		{
			EWeaponState::Drawn,
			FGameplayTagContainer::CreateFromArray(
				TArray{ TAG(Weapon.State.Drawing), TAG(Weapon.State.Drawn) })
		},
		{
			EWeaponState::Sheathed,
			FGameplayTagContainer::CreateFromArray(
				TArray{ TAG(Weapon.State.Sheathing), TAG(Weapon.State.Sheathed) })
		},
	};
	return Containers.FindChecked(WeaponState);
}

EWeaponState UWeaponStateLibrary::GetWeaponStateFromGameplayTag(const FGameplayTag& Tag, bool& bValid)
{
	// Set default values
	EWeaponState WeaponState = EWeaponState::Sheathed;
	bValid = true;

	// Drawing, Drawn -> Drawn
	// Sheathing, Sheathed -> Sheathed

	if (Tag.MatchesAnyExact(GetGameplayTagsForWeaponState(EWeaponState::Drawn)))
	{
		WeaponState = EWeaponState::Drawn;
	}
	else if (Tag.MatchesAnyExact(GetGameplayTagsForWeaponState(EWeaponState::Sheathed)))
	{
		WeaponState = EWeaponState::Sheathed;
	}
	else
	{
		bValid = false;
	}

	return WeaponState;
}

EWeaponState UWeaponStateLibrary::GetWeaponStateFromTagContainer(const FGameplayTagContainer& Container)
{
	if (Container.HasAny(GetGameplayTagsForWeaponState(EWeaponState::Drawn)))
	{
		return EWeaponState::Drawn;
	}

	// Default to Sheathed
	return EWeaponState::Sheathed;
}

EWeaponState UWeaponStateLibrary::GetWeaponStateFromTagOwner(
	const TScriptInterface<IGameplayTagAssetInterface> TagOwner)
{
	check(TagOwner);

	if (TagOwner->HasAnyMatchingGameplayTags(GetGameplayTagsForWeaponState(EWeaponState::Drawn)))
	{
		return EWeaponState::Drawn;
	}

	// Default to Sheathed
	return EWeaponState::Sheathed;
}
