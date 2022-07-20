// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "AnimNotify_WeaponActorPosition.generated.h"

enum class EWeaponState : uint8;
class UAnimInstance;
class UItemInstance;

/**
 * Animation notify that is used to place the weapon actor at the specified socket
 *
 * This also takes care of linking/unlinking anim BP classes according to the held weapon. This can be disabled.
 */
UCLASS(meta = (DisplayName = "Set Weapon Actor Position"))
class REMEMBERINGWALLACE_API UAnimNotify_WeaponActorPosition final : public UAnimNotify
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UAnimNotify_WeaponActorPosition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// BEGIN UAnimNotify interface
	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
	// END UAnimNotify interface

protected:
	/**
	 * The position the weapon actor should be attached to
	 *
	 * This is ignored when using a custom socket. However, it will determine if the anim BP class will be linked or
	 * unlinked in case custom anim BP class is not enabled and bAdjustAnimClassLink is set to true. Drawn will link the
	 * anim BP, while Sheathed will unlink it.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponState WeaponActorPosition;

	/**
	 * If set to true, the custom socket name will be used instead of sheathed/drawn positions
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (InlineEditConditionToggle))
	bool bUseCustomSocket;

	/**
	 * Name of the custom socket to attach to
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (EditCondition = "bUseCustomSocket"))
	FName CustomSocketName;

	/**
	 * If set to true, the custom anim BP class will be used to link/unlink instead of the one from the weapon
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (InlineEditConditionToggle))
	bool bUseCustomAnimClass;

	/**
	 * Custom anim BP class to link/unlink, ignored when not set
	 *
	 * You can enable this and set the value to none to disable anim BP linking as a whole.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (EditCondition = "bUseCustomAnimClass"))
	TSubclassOf<UAnimInstance> CustomAnimClassToLink;

	/**
	 * Whether to link or unlink the anim BP class set below
	 *
	 * If true, the anim BP class is linked. If false, it is unlinked. To disable this feature, simply set
	 * CustomAnimClassToLink to None.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (EditCondition = "bUseCustomAnimClass"))
	bool bLinkCustomAnimClass;

private:
	/**
	 * Figure out what name to use for the socket
	 */
	FName GetSocketNameToUse(const UItemInstance* CurrentWeapon) const;

	/**
	 * Link/unlink the anim BP class or just don't do anything according to the settings
	 */
	void AdjustAnimClassLink(const UItemInstance* CurrentWeapon, USkeletalMeshComponent* MeshComp) const;
};
