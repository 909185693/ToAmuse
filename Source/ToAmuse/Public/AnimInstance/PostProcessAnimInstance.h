// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Animation/AnimInstance.h"
#include "PostProcessAnimInstance.generated.h"


/**
 * UPostProcessAnimInstance
 */
UCLASS()
class TOAMUSE_API UPostProcessAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()
	
public:
	// the below functions are the native overrides for each phase
	virtual	void NativeInitializeAnimation() override;

	// for the bulk of the work to be done in NativeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY()
	class ATCharacter* OwnerPawn;

	/** ∆Ù”√FootIK */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	uint32 bEnableFootIK : 1;

	/** Left foot target */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float LeftFootTarget;

	/** Right foot target */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float RightFootTarget;

	/** Left foot location */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	FVector LeftFootLocation;

	/** Right foot location */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	FVector RightFootLocation;
};
