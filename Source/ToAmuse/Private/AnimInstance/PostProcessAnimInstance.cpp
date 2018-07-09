// Copyright 2018 by January. All Rights Reserved.

#include "PostProcessAnimInstance.h"
#include "TCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


UPostProcessAnimInstance::UPostProcessAnimInstance(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOffsetFastInterpSpeed = 25.f;
	ZOffsetSlowInterpSpeed = 10.f;
	RotationOffsetInterpSpeed = 15.f;

	ZOffsetMaxLimit = 30.f;
	ZOffsetMinLimit = -50.f;
	RollOffsetMaxLimit = 30.f;
	RollOffsetMinLimit = -30.f;
	PitchOffsetMaxLimit = 30.f;
	PitchOffsetMinLimit = -30.f;

	TraceStartHeight = 50.f;
	TraceLengthBelowCapsule = 75.f;
}

void UPostProcessAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	USkeletalMeshComponent* OwnerComponent = GetSkelMeshComponent();
	OwnerPawn = OwnerComponent ? Cast<ATCharacter>(OwnerComponent->GetOwner()) : nullptr;
}

void UPostProcessAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	class UCharacterMovementComponent* MovementComponent = OwnerPawn ? OwnerPawn->GetCharacterMovement() : nullptr;
	if (MovementComponent && MovementComponent->IsMovingOnGround())
	{
		bEnableFootIK = true;
	}
	else
	{
		bEnableFootIK = false;
	}

	if (bEnableFootIK)
	{
		float LeftTraceZOffset = 0.f;
		float RightTraceZOffset = 0.f;

		TraceFootIK(DeltaSeconds, LeftFootName, RightFootTarget, LeftTraceZOffset, LeftFootZOffset, LeftFootTarget, LeftFootRotationOffset);
		TraceFootIK(DeltaSeconds, RightFootName, LeftFootTarget, RightTraceZOffset, RightFootZOffset, RightFootTarget, RightFootRotationOffset);

		const float TargetPelvisZOffset = LeftTraceZOffset <= RightTraceZOffset ? LeftFootTarget : RightFootTarget;

		PelvisZOffset = FMath::FInterpTo(PelvisZOffset, TargetPelvisZOffset, DeltaSeconds, ZOffsetSlowInterpSpeed);
	}
}

bool UPostProcessAnimInstance::TraceFootIK(float DeltaTime, const FName& FootName, const float OtherFootTarget, float& OutTraceZOffset, float& OutFootZOffset, float& OutFootTarget, FRotator& OutFootRotationOffset)
{
	class USkeletalMeshComponent* OwnerComponent = GetSkelMeshComponent();
	if (OwnerComponent != nullptr)
	{
		const FVector& ComponentLocation = OwnerComponent->GetComponentLocation();
		const FVector& FootLocation = OwnerComponent->GetSocketLocation(FootName);

		const FVector StartTrace(FootLocation.X, FootLocation.Y, ComponentLocation.Z + TraceStartHeight);
		const FVector EndTrace(FootLocation.X, FootLocation.Y, ComponentLocation.Z - TraceLengthBelowCapsule);

		FHitResult HitResult;
		const bool bFootTraceHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility);

		const float Roll = FMath::Clamp(180.f / PI * FMath::Atan2(HitResult.ImpactNormal.Y, HitResult.ImpactNormal.Z), RollOffsetMinLimit, RollOffsetMaxLimit);
		const float Pitch = FMath::Clamp(180.f / PI * FMath::Atan2(HitResult.ImpactNormal.X, HitResult.ImpactNormal.Z), PitchOffsetMinLimit, PitchOffsetMaxLimit) * -1.f;

		const FRotator TargetRotation(Pitch, 0.f, Roll);

		OutFootRotationOffset = FMath::RInterpTo(OutFootRotationOffset, TargetRotation, DeltaTime, RotationOffsetInterpSpeed);
		OutTraceZOffset = FMath::Clamp(HitResult.Location.Z - ComponentLocation.Z, ZOffsetMinLimit, ZOffsetMaxLimit);
		OutFootTarget = bFootTraceHit ? OutTraceZOffset : 0.f;

		const float ZOffsetInterpSpeed = OutFootTarget > OtherFootTarget ? ZOffsetFastInterpSpeed : (OutFootZOffset < OutFootTarget ? ZOffsetFastInterpSpeed : ZOffsetSlowInterpSpeed);

		OutFootZOffset = FMath::FInterpTo(OutFootZOffset, OutFootTarget, DeltaTime, ZOffsetInterpSpeed);

		return bFootTraceHit;
	}

	return false;
}