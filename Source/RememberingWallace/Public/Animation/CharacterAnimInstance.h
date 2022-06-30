// Elite Centennial

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"

#include "CharacterAnimInstance.generated.h"

class UCharacterBaseMovementComponent;
class ACharacterBase;
struct FCharacterAnimInstanceProxy;

/**
 * The base class for animation blueprints for the characters in this project
 *
 * This is intended for ACharacterBase actor class. This code is adapted from the Lyra Starter Game sample project.
 */
UCLASS(Abstract)
class REMEMBERINGWALLACE_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	/**
	 * Constructor
	 */
	UCharacterAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	/**
	 * 1-to-1 mapping from gameplay tags owned by the owning actor to properties on the blueprint class
	 *
	 * The values for the properties will be automatically updated as the specified tags are added or removed. This is
	 * more efficient than manually querying for the tags every time the information is needed.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	/**
	 * Information about how far the ground is below the character
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float GroundDistance;

	/**
	 * Get the owning actor as the character class used in this project
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	const ACharacterBase* GetCharacter() const;

	/**
	 * Get the owning actor's character movement component as the class used in this project
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	const UCharacterBaseMovementComponent* GetCharacterMovement() const;

	// BEGIN UAnimInstance interface
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override;
	// END UAnimInstance interface

	// BEGIN UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	// END UObject interface
};

/**
 * Customized proxy object for UCharacterAnimInstance
 */
USTRUCT()
struct REMEMBERINGWALLACE_API FCharacterAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

	FCharacterAnimInstanceProxy() : Super()
	{
	}

	explicit FCharacterAnimInstanceProxy(UCharacterAnimInstance* AnimInstance) : Super(AnimInstance)
	{
	}
};
