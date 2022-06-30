// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "AbilityInputID.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"
#include "Engine/DataAsset.h"

#include "AbilitySet.generated.h"

class UAbilitySet;
class UAttributeSetBase;
class UGameplayAbility;
class UGameplayEffect;
class UWallaceAbilitySystemComponent;

/**
 * Information about an attribute set to include in an ability set
 *
 * This code is adapted from Lyra Starter Game sample project.
 */
USTRUCT(BlueprintType)
struct FAbilitySet_AttributeSet
{
	GENERATED_BODY()

	/**
	 * The attribute set to add
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSetBase> AttributeSet;

	/**
	 * The name to give to the added attribute set
	 *
	 * If not provided, a default name will be given when added to the ASC.
	 */
	UPROPERTY(EditDefaultsOnly)
	FName AttributeSetName;
};

/**
 * Information about a gameplay effect to include in an ability set
 *
 * This code is adapted from Lyra Starter Game sample project.
 */
USTRUCT(BlueprintType)
struct FAbilitySet_GameplayEffect
{
	GENERATED_BODY()

	/**
	 * The gameplay effect to apply
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	/**
	 * The level of the gameplay effect
	 */
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel;
};

/**
 * Information about a gameplay ability to include in an ability set
 *
 * This code is adapted from Lyra Starter Game sample project.
 */
USTRUCT(BlueprintType)
struct FAbilitySet_GameplayAbility
{
	GENERATED_BODY()

	/**
	 * The gameplay ability to grant
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> GameplayAbility;

	/**
	 * The level of the gameplay ability
	 */
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel;

	/**
	 * Player input this ability is bound to
	 */
	UPROPERTY(EditDefaultsOnly)
	EAbilityInputID InputID;
};

/**
 * Handle to a granted ability set
 *
 * The caller of UMSAbilitySet::Grant must store the returned handles itself and call TakeBack method on the handle when
 * necessary.
 *
 * This code is adapted from Lyra Starter Game sample project.
 */
USTRUCT(BlueprintType)
struct FAbilitySetGrantedHandles
{
	GENERATED_BODY()

	// Allow the ability set to use private functions
	friend UAbilitySet;

	/**
	 * Take back all of the granted GAs, GEs, and attribute sets from the associated ASC
	 *
	 * The order of removal is GAs -> GEs -> attribute sets. This will clear all arrays.
	 */
	void TakeBack();

	/**
	 * Whether the associated ASC is valid
	 */
	bool IsValid() const { return AbilitySystemComponent.IsValid(); }

	/**
	 * Get the ASC the ability set was granted to
	 */
	UWallaceAbilitySystemComponent* GetAbilitySystemComponent() { return AbilitySystemComponent.Get(); }
	const UWallaceAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent.Get(); }

	/**
	 * Get the list of added attribute sets
	 */
	const TArray<TWeakObjectPtr<UAttributeSetBase>>& GetAttributeSets() const { return AttributeSets; }

	/**
	 * Get the list of handles to the applied gameplay effects
	 */
	const TArray<FActiveGameplayEffectHandle>& GetGameplayEffectHandles() const { return GameplayEffectHandles; }

	/**
	 * Get the list of handles to the granted gameplay abilities
	 */
	const TArray<FGameplayAbilitySpecHandle>& GetAbilitySpecHandles() const { return AbilitySpecHandles; }

protected:
	/**
	 * Pointers to the added attribute sets
	 *
	 * We want to use weak pointers because we do not want to prevent the attribute sets from being garbage collected.
	 */
	UPROPERTY()
	TArray<TWeakObjectPtr<UAttributeSetBase>> AttributeSets;

	/**
	 * Handles to the applied gameplay effects
	 */
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	/**
	 * Handles to the granted ability specs
	 */
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	/**
	 * Weak pointer to the ASC the ability set was granted to
	 *
	 * We want to use a weak pointer because we do not want to prevent the ASC from being garbage collected.
	 */
	UPROPERTY()
	TWeakObjectPtr<UWallaceAbilitySystemComponent> AbilitySystemComponent;

private:
	/**
	 * Add the pointer to the array
	 */
	void AddAttributeSet(UAttributeSetBase* AttributeSet);

	/**
	 * Add the handle to the array
	 */
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);

	/**
	 * Add the handle to the array
	 */
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);

	/**
	 * Clear and reset all arrays
	 */
	void Clear();
};

/**
 * Collection of GAs, GEs, and attribute sets to be granted to an ASC
 *
 * This code is adapted from Lyra Starter Game sample project.
 */
UCLASS(BlueprintType, Const)
class REMEMBERINGWALLACE_API UAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * Grant the GAs, GEs, and attribute sets to the ASC
	 *
	 * The granted order is attribute sets -> GEs -> GAs. If the pointer to a valid FAbilitySetGrantedHandles is
	 * provided, it has to be for the same ASC. Otherwise, this function will do nothing and return false. If the
	 * provided pointer has an invalid one, all granted handles stored in the struct will be cleared out. If you don't
	 * want the arrays to be overwritten, make sure you check if the struct you provide returns true for IsValid method.
	 */
	bool Grant(
		UWallaceAbilitySystemComponent* AbilitySystemComponent,
		FAbilitySetGrantedHandles* OutHandles = nullptr) const;

	/**
	 * Grant the GAs, GEs, and attribute sets to the ASC
	 *
	 * The granted order is attribute sets -> GEs -> GAs.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = "false", Category = "AbilitySet",
		DisplayName = "Grant to Ability System component")
	void BP_Grant(
		UWallaceAbilitySystemComponent* AbilitySystemComponent,
		FAbilitySetGrantedHandles& OutHandles) const;

protected:
	/**
	 * Attribute sets to add
	 */
	UPROPERTY(EditDefaultsOnly, meta = (TitleProperty = "AttributeSet"))
	TArray<FAbilitySet_AttributeSet> AttributeSets;

	/**
	 * Gameplay effects to apply
	 */
	UPROPERTY(EditDefaultsOnly, meta = (TitleProperty = "GameplayEffect"))
	TArray<FAbilitySet_GameplayEffect> GameplayEffects;

	/**
	 * Gameplay abilities to grant
	 */
	UPROPERTY(EditDefaultsOnly, meta = (TitleProperty = "GameplayAbility"))
	TArray<FAbilitySet_GameplayAbility> GameplayAbilities;
};
