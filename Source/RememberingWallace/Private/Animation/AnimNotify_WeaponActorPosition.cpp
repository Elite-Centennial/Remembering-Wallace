// Elite Centennial

#include "Animation/AnimNotify_WeaponActorPosition.h"

#include "ItemInstance.h"
#include "Character/CharacterBase.h"
#include "Items/EquipmentManagerComponent.h"
#include "Items/ItemProperty_Weapon.h"
#include "Items/WeaponState.h"
#include "Items/WeaponItemActor.h"

UAnimNotify_WeaponActorPosition::UAnimNotify_WeaponActorPosition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(178, 255, 156, 255);
#endif

	WeaponActorPosition = EWeaponState::Sheathed;
	bUseCustomSocket = false;
	bUseCustomAnimClass = false;
	bLinkCustomAnimClass = false;
}

void UAnimNotify_WeaponActorPosition::Notify(
	USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner());
	const UEquipmentManagerComponent* EquipmentManager = Character ? Character->GetEquipmentManager() : nullptr;
	const UItemInstance* CurrentWeapon = EquipmentManager ? EquipmentManager->GetCurrentWeapon() : nullptr;
	const FItemPropertyData_Weapon* WeaponData = CurrentWeapon ? CurrentWeapon->GetWeaponPropertyData() : nullptr;
	AWeaponItemActor* WeaponActor = WeaponData ? WeaponData->SpawnedActor.Get() : nullptr;

	if (!WeaponActor || MeshComp != WeaponActor->GetRootComponent()->GetAttachParent())
	{
		return;
	}

	// Attach to the component using the given socket name
	const FName SocketName = GetSocketNameToUse(CurrentWeapon);
	WeaponActor->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

	// Link/unlink anim BP class
	AdjustAnimClassLink(CurrentWeapon, MeshComp);
}

FName UAnimNotify_WeaponActorPosition::GetSocketNameToUse(const UItemInstance* CurrentWeapon) const
{
	if (bUseCustomSocket) {
		return CustomSocketName;
	}

	const UItemProperty_Weapon* WeaponProperty = CurrentWeapon->GetWeaponProperty();
	check(WeaponProperty); // We already know the item instance has weapon property data, so this must be non-null

	return WeaponProperty->GetActorPositionSocketName(WeaponActorPosition);
}

void UAnimNotify_WeaponActorPosition::AdjustAnimClassLink(
	const UItemInstance* CurrentWeapon, USkeletalMeshComponent* MeshComp) const
{
	// The anim BP class to use; we're going to do nothing if this is not set
	TSubclassOf<UAnimInstance> AnimClass;
	// Whether to link or unlink the anim class; link if true, unlink if false
	bool bShouldLink;

	if (bUseCustomAnimClass)
	{
		// If using a custom anim BP class, use the settings on this anim notify instance
		AnimClass = CustomAnimClassToLink;
		bShouldLink = bLinkCustomAnimClass;
	}
	else
	{
		// If not using a custom anim BP class, retrieve the anim BP class from the weapon property
		const UItemProperty_Weapon* WeaponProperty = CurrentWeapon->GetWeaponProperty();
		check(WeaponProperty); // We already know the item instance has weapon property data, so this must be non-null
		AnimClass = WeaponProperty->GetAnimClassToLink();
		bShouldLink = WeaponActorPosition == EWeaponState::Drawn; // Link if drawn out, unlink if sheathed
	}

	// Don't do anything if the anim BP class is not set
	if (!AnimClass)
	{
		return;
	}

	// Link or unlink the anim BP class
	if (bShouldLink)
	{
		MeshComp->LinkAnimClassLayers(AnimClass);
	}
	else
	{
		MeshComp->UnlinkAnimClassLayers(AnimClass);
	}
}
