// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Animation/AnimInstance.h"
#include "TAnimInstance.generated.h"


UENUM(BlueprintType)
namespace EMovementDirection
{
	enum Type
	{
		Fwd,
		Bwd,
		Left,
		Right
	};
}

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

	UFUNCTION(BlueprintCallable, Category="Animation")
	const TEnumAsByte<EMovementDirection::Type> ConvertDirection(float NewDirection) const;

protected:
	UPROPERTY()
	class ATCharacter* OwnerPawn;

	/** 角色速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (ClampMin = "0"))
	float Speed;

	/** 运动方向 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (ClampMin = "-180.0", ClampMax = "180.0"))
	float Direction;
	
	/** 运动方向 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TEnumAsByte<EMovementDirection::Type> MovementDirection;

	/** 运动模式 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TEnumAsByte<EMovementMode> MovementMode;

	/** 下落速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float FallSpeed;

	/** 是否疾跑 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	uint32 bIsSprinting : 1;

};
