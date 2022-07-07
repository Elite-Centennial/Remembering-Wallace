// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "AnimNotify_WeaponActorPosition.generated.h"

class UAnimInstance;
class UItemInstance;

/**
 * How to figure out the name of the socket the weapon actor should be attached to
 */
UENUM(BlueprintType, Category = "Weapon|Animation")
enum class EWeaponActorPosition : uint8
{
	// Use SheathedSocketName defined in the weapon item property
	Sheathed,
	// Use DrawnSocketName defined in the weapon item property
	Drawn,
};

/**
 * Animation notify that is used to place the weapon actor at the specified socket
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
	 * This is ignored when using a custom socket.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponActorPosition WeaponActorPosition;

	/**
	 * Whether the appropriate anim BP class should be automatically linked or unlinked
	 *
	 * This is ignored when using a custom socket.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bAdjustAnimClassLink;

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
	 * Whether to link or unlink the anim BP class set below
	 *
	 * If true, the anim BP class is linked. If false, it is unlinked. To disable this feature, simply set the
	 * CustomAnimClassToLink to None.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (EditCondition = "bUseCustomSocket"))
	bool bLinkCustomAnimClass;

	/**
	 * Anim BP class to link/unlink, ignored when not set
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (EditCondition = "bUseCustomSocket"))
	TSubclassOf<UAnimInstance> CustomAnimClassToLink;

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
