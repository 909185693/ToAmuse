// Copyright 2018 by January. All Rights Reserved.

#include "TAnimInstance.h"
#include "TCharacter.h"
#include "AdvancedMovementComponent.h"
#include "ToAmuse.h"


UTAnimInstance::UTAnimInstance(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TurnInPlaceSlot = TEXT("TurnInPlaceSlot");
	TurnMinAngle = 60.f;
	TurnAngleThreshold = 130.f;
}

void UTAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	USkeletalMeshComponent* OwnerComponent = GetSkelMeshComponent();
	OwnerPawn = OwnerComponent ? Cast<ATCharacter>(OwnerComponent->GetOwner()) : nullptr;

	if (OwnerPawn != nullptr)
	{
		CharacterRotation = OwnerPawn->GetActorRotation();
	}
}

void UTAnimInstance::UpdateMeshRotation(float DeltaSeconds)
{
	if (OwnerPawn != nullptr)
	{
		const FRotator& NewRotation = CharacterRotation + FRotator(0.f, -90.f, 0.f);

		OwnerPawn->GetMesh()->SetWorldRotation(NewRotation);
	}
}

void UTAnimInstance::SetCharacterRotation(const FRotator& NewRotation, bool bInterpRotation, float InterpSpeed, float DeltaSeconds)
{
	if (bInterpRotation)
	{
		CharacterRotation = FMath::RInterpTo(CharacterRotation, NewRotation, DeltaSeconds, InterpSpeed);
	}
	else
	{
		CharacterRotation = NewRotation;
	}
}

bool UTAnimInstance::WasWhetherRange(float Value, float MinRangeTrue, float MaxRangeTrue, float MinRangeFalse, float MaxRangeFlase, bool bWhether)
{
	return (Value >= (bWhether ? MinRangeTrue : MinRangeFalse)) && (Value <= (bWhether ? MaxRangeTrue : MaxRangeFlase));
}

void UTAnimInstance::InAirTrace(float& OutPredictedInAirTime, bool& OutUsePredictedAirTime, float& OutInAirTime) const
{
	class UCapsuleComponent* CapsuleComponent = OwnerPawn ? OwnerPawn->GetCapsuleComponent() : nullptr;
	if (CapsuleComponent != nullptr)
	{
		FPredictProjectilePathParams PredictProjectilePathParams;
		PredictProjectilePathParams.StartLocation = CapsuleComponent->GetComponentLocation() - FVector(0.f, 0.f, CapsuleComponent->GetScaledCapsuleHalfHeight_WithoutHemisphere());
		PredictProjectilePathParams.LaunchVelocity = OwnerPawn->GetVelocity();
		PredictProjectilePathParams.ProjectileRadius = CapsuleComponent->GetScaledCapsuleRadius() - 5.f;
		PredictProjectilePathParams.bTraceWithCollision = true;
		PredictProjectilePathParams.bTraceWithChannel = true;
		PredictProjectilePathParams.TraceChannel = ECollisionChannel::ECC_Visibility;
		PredictProjectilePathParams.SimFrequency = 10.f;
		PredictProjectilePathParams.MaxSimTime = 3.f;

		FPredictProjectilePathResult PredictProjectilePathResult;

		UGameplayStatics::PredictProjectilePath(GetWorld(), PredictProjectilePathParams, PredictProjectilePathResult);

		OutPredictedInAirTime = PredictProjectilePathResult.LastTraceDestination.Time;
		OutUsePredictedAirTime = (PredictProjectilePathResult.HitResult.bBlockingHit && PredictProjectilePathResult.HitResult.ImpactNormal.Z >= OwnerPawn->GetCharacterMovement()->GetWalkableFloorZ()) || !PredictProjectilePathResult.HitResult.bBlockingHit;
		OutInAirTime = 0.f;
	}
}

void UTAnimInstance::PlayTurnInPlaceAnimation(float PlayRate, float BlendInTime, class UAnimSequence* TurnAnim, class UCurveFloat* TurnCurve)
{
	StartTurnInPlace();

	TurnInPlaceCurve = TurnCurve;
	TurnInPlaceTime = 0.f;
	CurveValue = 0.f;
	PreviousCurveValue = 0.f;
	TurnInPlaceRate = PlayRate;
	
	PlaySlotAnimationAsDynamicMontage(TurnAnim, TurnInPlaceSlot, BlendInTime, 0.2f, TurnInPlaceRate, 1, 0.f, 0.f);
}

void UTAnimInstance::TurnInPlaceCheck(float DeltaSeconds)
{
	const float AbsOffset = FMath::Abs(RotationOffset);

	if (AbsOffset > TurnMinAngle)
	{
		TurnInPlaceDelayCount += DeltaSeconds;

		if (TurnInPlaceDelayCount > FMath::GetMappedRangeValueClamped(FVector2D(45.f, 160.f), FVector2D(0.5f, 0.1f), AbsOffset))
		{
			if (RotationOffset > 0.f)
			{
				if (AbsOffset > TurnAngleThreshold)
				{
					PlayTurnInPlaceAnimation(1.5f, 0.15f, Turn180LeftAnim, Turn180LeftCurve);
				}
				else
				{
					PlayTurnInPlaceAnimation(1.25f, 0.15f, Turn90LeftAnim, Turn90LeftCurve);
				}
			}
			else
			{
				if (AbsOffset > TurnAngleThreshold)
				{
					PlayTurnInPlaceAnimation(1.5f, 0.15f, Turn180RightAnim, Turn180RightCurve);
				}
				else
				{
					PlayTurnInPlaceAnimation(1.25f, 0.15f, Turn90RightAnim, Turn90RightCurve);
				}
			}
		}
	}
	else
	{
		TurnInPlaceDelayCount = 0.f;
	}
}

void UTAnimInstance::StartTurnInPlace()
{
	bTurningInPlace = true;
}

void UTAnimInstance::StopTurnInPlace()
{
	bTurningInPlace = false;

	StopSlotAnimation(0.2f, TurnInPlaceSlot);
}

void UTAnimInstance::TurnInPlace(float DeltaSeconds)
{
	TurnInPlaceTime += DeltaSeconds;

	if (TurnInPlaceCurve != nullptr)
	{
		float CurrentCurveValue = TurnInPlaceCurve->GetFloatValue(TurnInPlaceTime * TurnInPlaceRate);

		CurveValue = CurrentCurveValue - PreviousCurveValue;

		PreviousCurveValue = CurrentCurveValue;

		FRotator TargetCharaterRotaion = CharacterRotation + FRotator(0.f, CurveValue, 0.f);

		SetCharacterRotation(TargetCharaterRotaion);
	}
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

	// 移动模式
	class UAdvancedMovementComponent* AdvancedMovement = OwnerPawn ? Cast<UAdvancedMovementComponent>(OwnerPawn->GetCharacterMovement()) : nullptr;
	if (AdvancedMovement != nullptr)
	{
		MovementMode = AdvancedMovement->MovementMode;
		PrevMovementMode = AdvancedMovement->PrevMovementMode;
		JumpZVelocity = AdvancedMovement->JumpZVelocity;

		if (MovementMode == EMovementMode::MOVE_Falling)
		{
			FallSpeed = Velocity.Z;
			InAirTime += DeltaSeconds;
			LeanInAir = FMath::GetMappedRangeValueClamped(FVector2D(JumpZVelocity, -JumpZVelocity), FVector2D(1.f, -1.f), FallSpeed) * (Speed / AdvancedMovement->MaxWalkSpeed);
		}

		// 是否移动
		bMoving = !FMath::IsNearlyZero(Speed, 1.0e-03f);

		// 是否输入
		bMovementInput = !AdvancedMovement->MovementInputVector.IsNearlyZero(1.0e-02f);

		// 是否疾跑
		bSprinting = OwnerPawn->bIsSprinting && Speed > AdvancedMovement->MaxWalkSpeed;
	}

	// 旋转度
	ActorRotation = OwnerPawn->GetActorRotation();

	BaseAimRotation = OwnerPawn->GetBaseAimRotation();

	if (bMoving)
	{
		StopTurnInPlace();

		float LastVelocityDeltaYaw = (ActorRotation - LastVelocityRotation).GetNormalized().Yaw;

		if (bSprinting)
		{
			CardinalDirection = ECardinalDirection::Forwards;

			MovementDirection = EMovementDirection::Forwards;

			RotationOffset = LastVelocityDeltaYaw;

			SetCharacterRotation(LastVelocityRotation, true, FMath::GetMappedRangeValueClamped(FVector2D(0.f, 600.f), FVector2D(1.f, 10.f), Speed), DeltaSeconds);
		}
		else
		{
			CardinalDirection = WasWhetherRange(LastVelocityDeltaYaw, -100.f, 100.f, -80.f, 80.f, CardinalDirection != ECardinalDirection::Backwards) ? ECardinalDirection::Forwards : ECardinalDirection::Backwards;

			MovementDirection = WasWhetherRange(LastVelocityDeltaYaw, -100.f, 100.f, -80.f, 80.f, MovementDirection == EMovementDirection::Forwards) ? EMovementDirection::Forwards : EMovementDirection::Backwards;

			const float TargetRotationOffset = CardinalDirection == ECardinalDirection::Backwards ? (LastVelocityDeltaYaw > 0.f ? LastVelocityDeltaYaw - 180.f : LastVelocityDeltaYaw + 180.f) : LastVelocityDeltaYaw;

			RotationOffset = FMath::FInterpTo(RotationOffset, TargetRotationOffset, DeltaSeconds, 5.f);

			SetCharacterRotation(ActorRotation + FRotator(0.f, RotationOffset, 0.f), true, FMath::GetMappedRangeValueClamped(FVector2D(165.f, 375.f), FVector2D(5.f, 8.f), Speed), DeltaSeconds);
		}

		LastVelocityRotation = FVector(Velocity.X, Velocity.Y, 0.f).Rotation();

		Direction = (LastVelocityRotation - CharacterRotation).GetNormalized().Yaw;
	}
	else
	{
		RotationOffset = (CharacterRotation - ActorRotation).GetNormalized().Yaw;

		if (!bTurningInPlace)
		{
			TurnInPlaceCheck(DeltaSeconds);
		}
	}

	const FRotator& AimOffsetRotation = (BaseAimRotation - CharacterRotation).GetNormalized();
	const float AimmOffsetInterpSpeed = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 180.f), FVector2D(15.f, 5.f), AimOffsetRotation.Yaw - AimOffset.X);
	AimOffset = FMath::Vector2DInterpTo(AimOffset, FVector2D(AimOffsetRotation.Yaw, AimOffsetRotation.Pitch), DeltaSeconds, AimmOffsetInterpSpeed);

	UpdateMeshRotation(DeltaSeconds);

	// LeanRotation
	YawValue = (LastVelocityRotation - PreviousVelocityRotation).GetNormalized().Yaw / DeltaSeconds;

	PreviousVelocityRotation = LastVelocityRotation;

	float TargetLeanRotation = FMath::GetMappedRangeValueClamped(FVector2D(-200.f, 200.f), FVector2D(-1.f, 1.f), FMath::Clamp(YawValue, -200.f, 200.f)) * FMath::GetMappedRangeValueClamped(FVector2D(165, 375.f), FVector2D(0.f, 1.f), Speed);

	float LeanSpeedRatio = FMath::GetMappedRangeValueClamped(FVector2D(165.f, 375.f), FVector2D(0, 1.f), Speed);

	LeanRotation = FMath::FInterpTo(LeanRotation, TargetLeanRotation * LeanSpeedRatio, DeltaSeconds, 8.f);

	// LeanAcceleration
	AccelerationValue = (Speed - PreviousSpeed) / DeltaSeconds;

	PreviousSpeed = Speed;

	const FVector2D LeanAccelerationInRange = AccelerationValue < 0.f ? FVector2D(0.f, -1.f * AdvancedMovement->BrakingDecelerationWalking) : FVector2D(0.f, AdvancedMovement->GetMaxAcceleration());
	const FVector2D LeanAccelerationOutRange = AccelerationValue < 0.f ? FVector2D(0.f, -1.f) : FVector2D(0.f, 1.f);

	LeanAcceleration = FMath::FInterpTo(LeanAcceleration, FMath::GetMappedRangeValueClamped(LeanAccelerationInRange, LeanAccelerationOutRange, AccelerationValue), DeltaSeconds, 8.f);

	// Curve
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