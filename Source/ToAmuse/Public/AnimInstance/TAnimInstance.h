// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Animation/AnimInstance.h"
#include "TAnimInstance.generated.h"


/**
 * TAnimInstance
 */
UCLASS()
class TOAMUSE_API UTAnimInstance : public UAnimInstance
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

	/** 角色速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (ClampMin = "0"))
	float Speed;

	/** 运动方向 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (ClampMin = "-180.0", ClampMax = "180.0"))
	float Direction;

	/** 旋转速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (ClampMin = "-180.0", ClampMax = "180.0"))
	float RotationSpeed;
	
};
