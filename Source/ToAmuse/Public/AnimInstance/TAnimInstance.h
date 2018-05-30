// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "Animation/AnimInstance.h"
#include "TAnimInstance.generated.h"


UENUM(BlueprintType)
namespace EMovementDirection
{
	enum Type
	{
		Forwards,
		Backwards
	};
}

UENUM(BlueprintType)
namespace ECardinalDirection
{
	enum Type
	{
		Forwards,
		Backwards,
		Leftwards,
		Rightwards
	};
}

UENUM(BlueprintType)
namespace EFootPlant
{
	enum Type
	{
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
	void ConvertDirection(float NewDirection, TEnumAsByte<EMovementDirection::Type>& OutMovementDirection, TEnumAsByte<ECardinalDirection::Type>& OutCardinalDirection) const;

protected:
	UPROPERTY()
	class ATCharacter* OwnerPawn;

	/** 是否移动 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	uint32 bMoving : 1;

	/** 是否输入 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	uint32 bMovementInput : 1;

	/** 角色速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (ClampMin = "0"))
	float Speed;

	/** 运动方向 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (ClampMin = "-180.0", ClampMax = "180.0"))
	float Direction;
	
	/** 运动方向 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TEnumAsByte<EMovementDirection::Type> MovementDirection;

	/** 运动方向 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TEnumAsByte<ECardinalDirection::Type> CardinalDirection;

	/** 运动模式 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TEnumAsByte<EMovementMode> MovementMode;

	/** 下落速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float FallSpeed;

	/** 在空中时间 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float InAirTime;

	/** 是否疾跑 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	uint32 bSprinting : 1;

	/** 脚步 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TEnumAsByte<EFootPlant::Type> FootPlant;

	/** 当前移动距离 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float MoveDistance;

protected:
	UFUNCTION()
	void AnimNotify_LeftPlant(UAnimNotify* Notify);

	UFUNCTION()
	void AnimNotify_RightPlant(UAnimNotify* Notify);
};
