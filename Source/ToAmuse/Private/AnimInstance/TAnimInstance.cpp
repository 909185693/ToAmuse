// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "TAnimInstance.h"
#include "TCharacter.h"
#include "AdvancedMovementComponent.h"


UTAnimInstance::UTAnimInstance(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UTAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	USkeletalMeshComponent* OwnerComponent = GetSkelMeshComponent();
	OwnerPawn = OwnerComponent ? Cast<ATCharacter>(OwnerComponent->GetOwner()) : nullptr;
}

void UTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerPawn == nullptr)
	{
		return;
	}

	// 设置角色速度
	const FVector Velocity = OwnerPawn->GetVelocity();

	Speed = Velocity.Size2D();

	// 设置角色运动方向
	//if (Speed > 0.f)
	//{
	//	Direction = CalculateDirection(Velocity, OwnerPawn->GetActorRotation());

	//	ConvertDirection(Direction, MovementDirection, CardinalDirection);
	//}

	// 移动模式
	class UAdvancedMovementComponent* AdvancedMovement = OwnerPawn ? Cast<UAdvancedMovementComponent>(OwnerPawn->GetCharacterMovement()) : nullptr;
	if (AdvancedMovement != nullptr)
	{
		MovementMode = AdvancedMovement->MovementMode;

		if (MovementMode == EMovementMode::MOVE_Falling)
		{
			FallSpeed = Velocity.Z;
			InAirTime += DeltaSeconds;
		}
		else
		{
			FallSpeed = 0.f;
			InAirTime = 0.f;
		}

		// 是否移动
		bMoving = !FMath::IsNearlyZero(Speed, 1.0e-03f);

		// 是否输入
		bMovementInput = !AdvancedMovement->MovementInputVector.IsNearlyZero(1.0e-02f);

		// 是否疾跑
		bSprinting = OwnerPawn->bIsSprinting && Speed > AdvancedMovement->MaxWalkSpeed;
	}

	// 旋转度
	if (bMoving)
	{
		CharacterRotation = OwnerPawn->GetActorRotation();

		LastVelocityRotation = FVector(Velocity.X, Velocity.Y, 0.f).Rotation();
		
		Direction = (LastVelocityRotation - CharacterRotation).GetNormalized().Yaw;

		ConvertDirection(Direction, MovementDirection, CardinalDirection);
	}

	// LeaningValues
	YawValue = (LastVelocityRotation - PreviousVelocityRotation).GetNormalized().Yaw / DeltaSeconds;

	PreviousVelocityRotation = LastVelocityRotation;

	float TargetLeanRotation = FMath::GetMappedRangeValueClamped(FVector2D(-200.f, 200.f), FVector2D(-1.f, 1.f), YawValue) * FMath::GetMappedRangeValueClamped(FVector2D(165, 375.f), FVector2D(0.f, 1.f), Speed);

	LeanRotation = 

	FootPosition = GetCurveValue(TEXT("FootPosition"));

	FootPositionDirection = GetCurveValue(TEXT("FootPositionDirection"));
}

void UTAnimInstance::ConvertDirection(float NewDirection, TEnumAsByte<EMovementDirection::Type>& OutMovementDirection, TEnumAsByte<ECardinalDirection::Type>& OutCardinalDirection) const
{
	if (NewDirection >= -45.f && NewDirection < 45.f)
	{
		OutCardinalDirection = ECardinalDirection::Forwards;
	}
	else if (NewDirection >= 45.f && NewDirection < 135.f)
	{
		OutCardinalDirection = ECardinalDirection::Rightwards;
	}
	else if (NewDirection >= -135.f && NewDirection < -45.f)
	{
		OutCardinalDirection = ECardinalDirection::Leftwards;
	}
	else
	{
		OutCardinalDirection = ECardinalDirection::Backwards;
	}

	if (NewDirection >= -90.f && NewDirection < 90.f)
	{
		OutMovementDirection = EMovementDirection::Forwards;
	}
	else
	{
		OutMovementDirection = EMovementDirection::Backwards;
	}
}

void UTAnimInstance::AnimNotify_LeftPlant(UAnimNotify* Notify)
{
	FootPlant = EFootPlant::Left;
}

void UTAnimInstance::AnimNotify_RightPlant(UAnimNotify* Notify)
{
	FootPlant = EFootPlant::Right;
}