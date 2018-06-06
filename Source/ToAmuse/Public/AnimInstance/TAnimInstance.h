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

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateMeshRotation(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetCharacterRotation(const FRotator& TargetRotation, bool bInterpRotation = false, float InterpSpeed = 0.f, float DeltaSeconds = 0.f);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	bool WasWhetherRange(float Value, float MinRangeTrue, float MaxRangeTrue, float MinRangeFalse, float MaxRangeFlase, bool bWhether);

protected:
	UPROPERTY()
	class ATCharacter* OwnerPawn;

	/** �Ƿ��ƶ� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	uint32 bMoving : 1;

	/** �Ƿ����� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	uint32 bMovementInput : 1;

	/** ��ɫ�ٶ� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (ClampMin = "0"))
	float Speed;

	/** �˶����� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (ClampMin = "-180.0", ClampMax = "180.0"))
	float Direction;
	
	/** �˶����� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TEnumAsByte<EMovementDirection::Type> MovementDirection;

	/** �˶����� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TEnumAsByte<ECardinalDirection::Type> CardinalDirection;

	/** �˶�ģʽ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TEnumAsByte<EMovementMode> MovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TEnumAsByte<EMovementMode> PrevMovementMode;

	/** �����ٶ� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float FallSpeed;

	/** �ڿ���ʱ�� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float InAirTime;

	/** ��Ծ�����ٶ� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float JumpZVelocity;

	/** �Ƿ�Ԥ�����ʱ�� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	uint32 bUsePredictedAirTime : 1;

	/** Ԥ�����ʱ�� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float PredictedInAirTime;
	
	/** �Ƿ��� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	uint32 bSprinting : 1;

	/** ƫ�� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	FVector2D AimOffset;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Rotator)
	FRotator ActorRotation;

	UPROPERTY(BlueprintReadOnly, Category = Rotator)
	FRotator BaseAimRotation;

	UPROPERTY(BlueprintReadOnly, Category = Rotator)
	FRotator LastVelocityRotation;
	
	UPROPERTY(BlueprintReadOnly, Category = Rotator)
	FRotator TargetRotation;

	UPROPERTY(BlueprintReadOnly, Category = Rotator)
	FRotator CharacterRotation;

	UPROPERTY(BlueprintReadOnly, Category = Rotator)
	uint32 bShouldRotateInPlace : 1;
	
	UPROPERTY(BlueprintReadOnly, Category = Rotator)
	float RotationOffset;

	UPROPERTY(BlueprintReadOnly, Category = Rotator)
	float LeanRotation;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Position)
	float FootPosition;

	UPROPERTY(BlueprintReadOnly, Category = Position)
	float FootPositionDirection;

protected:
	UPROPERTY(BlueprintReadOnly, Category = LeaningValues)
	FRotator PreviousVelocityRotation;

	UPROPERTY(BlueprintReadOnly, Category = LeaningValues)
	float YawValue;

	UPROPERTY(BlueprintReadOnly, Category = LeaningValues)
	float PreviousSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = LeaningValues)
	float AccelerationValue;

	UPROPERTY(BlueprintReadOnly, Category = LeaningValues)
	float LeanAcceleration;

protected:
	UFUNCTION()
	void AnimNotify_Falling();
};
