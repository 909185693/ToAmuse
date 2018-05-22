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
	
	bool TraceFootIK(float DeltaTime, const FName& FootName, const float OtherFootTarget, float& OutTraceZOffset, float& OutFootZOffset, float& OutFootTarget, FRotator& OutFootRotationOffset);

protected:
	UPROPERTY()
	class ATCharacter* OwnerPawn;

	/** Pelvis Z offset */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Offsets)
	float PelvisZOffset;

	/** Left foot Z offset */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Offsets)
	float LeftFootZOffset;

	/** Right foot Z offset */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Offsets)
	float RightFootZOffset;

	/** Left foot rotation offset */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Offsets)
	FRotator LeftFootRotationOffset;

	/** Right foot rotation offset */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Offsets)
	FRotator RightFootRotationOffset;

	/** Z offset max limit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OffsetLimits)
	float ZOffsetMaxLimit;

	/** Z offset min limit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OffsetLimits)
	float ZOffsetMinLimit;

	/** Roll offset max limit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OffsetLimits)
	float RollOffsetMaxLimit;

	/** Roll offset min limit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OffsetLimits)
	float RollOffsetMinLimit;

	/** Pitch offset max limit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OffsetLimits)
	float PitchOffsetMaxLimit;

	/** Pitch ffset min limit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OffsetLimits)
	float PitchOffsetMinLimit;

	/** Z offset fast interp speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OffsetInterpSpeeds)
	float ZOffsetFastInterpSpeed;

	/** Z offset slow interp speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OffsetInterpSpeeds)
	float ZOffsetSlowInterpSpeed;

	/** Rotation offset interp speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OffsetInterpSpeeds)
	float RotationOffsetInterpSpeed;

	/** Left foot name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Names)
	FName LeftFootName;

	/** Right foot name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Names)
	FName RightFootName;

	/** ∆Ù”√FootIK */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TraceSettings)
	uint32 bEnableFootIK : 1;

	/** Trace start height */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TraceSettings)
	float TraceStartHeight;

	/** Trace length below capsule */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TraceSettings)
	float TraceLengthBelowCapsule;

	/** Left foot target */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TraceSettings)
	float LeftFootTarget;

	/** Right foot target */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TraceSettings)
	float RightFootTarget;

	/** Left foot location */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TraceSettings)
	FVector LeftFootLocation;

	/** Right foot location */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TraceSettings)
	FVector RightFootLocation;
};
