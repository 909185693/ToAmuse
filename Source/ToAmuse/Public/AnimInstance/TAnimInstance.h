// Copyright 2018 by January. All Rights Reserved.

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

USTRUCT(BlueprintType)
struct FFallingAnimationParameters
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameters)
	class UAnimSequence* AnimSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameters)
	float StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameters)
	float EndTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameters)
	float ApexTime;
};

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

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateMeshRotation(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetCharacterRotation(const FRotator& NewRotation, bool bInterpRotation = false, float InterpSpeed = 0.f, float DeltaSeconds = 0.f);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	bool WasWhetherRange(float Value, float MinRangeTrue, float MaxRangeTrue, float MinRangeFalse, float MaxRangeFlase, bool bWhether);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void InAirTrace(float& OutPredictedInAirTime, bool& OutUsePredictedAirTime, float& OutInAirTime) const;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayTurnInPlaceAnimation(float PlayRate, float BlendInTime, class UAnimSequence* TurnAnim, class UCurveFloat* TurnCurve);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void TurnInPlaceCheck(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StartTurnInPlace();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StopTurnInPlace();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void TurnInPlace(float DeltaSeconds);

protected:
	UPROPERTY()
	class ATCharacter* OwnerPawn;

	/** 是否移动 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	uint32 bMoving : 1;

	/** 是否输入 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	uint32 bMovementInput : 1;

	/** 角色速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (ClampMin = "0"))
	float Speed;

	/** 运动方向 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (ClampMin = "-180.0", ClampMax = "180.0"))
	float Direction;
	
	/** 运动方向 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	TEnumAsByte<EMovementDirection::Type> MovementDirection;

	/** 运动方向 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	TEnumAsByte<ECardinalDirection::Type> CardinalDirection;

	/** 运动模式 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	TEnumAsByte<EMovementMode> MovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	TEnumAsByte<EMovementMode> PrevMovementMode;

	/** 下落速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float FallSpeed;

	/** 在空中时间 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float InAirTime;

	/** 空中倾斜度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float LeanInAir;
	
	/** 跳跃发射速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float JumpZVelocity;

	/** 是否预测空中时间 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	uint32 bUsePredictedAirTime : 1;

	/** 预测空中时间 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float PredictedInAirTime;
	
	/** 是否疾跑 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	uint32 bSprinting : 1;

	/** 偏移 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	FVector2D AimOffset;

protected:
	UPROPERTY(BlueprintReadWrite, Category = Rotator)
	FRotator ActorRotation;

	UPROPERTY(BlueprintReadWrite, Category = Rotator)
	FRotator BaseAimRotation;

	UPROPERTY(BlueprintReadWrite, Category = Rotator)
	FRotator LastVelocityRotation;
	
	UPROPERTY(BlueprintReadWrite, Category = Rotator)
	FRotator TargetRotation;

	UPROPERTY(BlueprintReadWrite, Category = Rotator)
	FRotator CharacterRotation;

	UPROPERTY(BlueprintReadWrite, Category = Rotator)
	FRotator PreviousCharacterRotation;
	
	UPROPERTY(BlueprintReadWrite, Category = Rotator)
	float RotationOffset;

	UPROPERTY(BlueprintReadWrite, Category = Rotator)
	float LeanRotation;

protected:
	UPROPERTY(BlueprintReadWrite, Category = Position)
	float FootPosition;

	UPROPERTY(BlueprintReadWrite, Category = Position)
	float FootPositionDirection;

protected:
	UPROPERTY(BlueprintReadWrite, Category = LeaningValues)
	FRotator PreviousVelocityRotation;

	UPROPERTY(BlueprintReadWrite, Category = LeaningValues)
	float YawValue;

	UPROPERTY(BlueprintReadWrite, Category = LeaningValues)
	float PreviousSpeed;
	
	UPROPERTY(BlueprintReadWrite, Category = LeaningValues)
	float AccelerationValue;

	UPROPERTY(BlueprintReadWrite, Category = LeaningValues)
	float LeanAcceleration;

protected:
	UPROPERTY(BlueprintReadWrite, Category = TemporaryValues)
	class UBlendSpaceBase* StoppingBlendSpace;

	UPROPERTY(BlueprintReadWrite, Category = TemporaryValues)
	struct FFallingAnimationParameters FallingAnimationParameters;

protected:
	UPROPERTY(BlueprintReadWrite, Category = Turn)
	FName TurnInPlaceSlot;

	UPROPERTY(BlueprintReadWrite, Category = Turn)
	UCurveFloat* TurnInPlaceCurve;
	
	UPROPERTY(BlueprintReadWrite, Category = Turn)
	uint32 bTurningInPlace : 1;

	UPROPERTY(BlueprintReadWrite, Category = Turn)
	float TurnInPlaceTime;

	UPROPERTY(BlueprintReadWrite, Category = Turn)
	float CurveValue;

	UPROPERTY(BlueprintReadWrite, Category = Turn)
	float PreviousCurveValue;

	UPROPERTY(BlueprintReadWrite, Category = Turn)
	float TurnInPlaceRate;

	UPROPERTY(BlueprintReadWrite, Category = Turn)
	float TurnInPlaceDelayCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turn)
	float TurnMinAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turn)
	float TurnAngleThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turn)
	class UAnimSequence* Turn90LeftAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turn)
	class UAnimSequence* Turn90RightAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turn)
	class UAnimSequence* Turn180LeftAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turn)
	class UAnimSequence* Turn180RightAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turn)
	class UCurveFloat* Turn90LeftCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turn)
	class UCurveFloat* Turn90RightCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turn)
	class UCurveFloat* Turn180LeftCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turn)
	class UCurveFloat* Turn180RightCurve;
};
