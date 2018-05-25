// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "TAnimInstance.h"
#include "TCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	if (Speed > 0.f)
	{
		Direction = CalculateDirection(Velocity, OwnerPawn->GetActorRotation());

		MovementDirection = ConvertDirection(Direction);
	}
		
	// 移动模式
	class UCharacterMovementComponent* MovementComponent = OwnerPawn ? OwnerPawn->GetCharacterMovement() : nullptr;
	if (MovementComponent != nullptr)
	{
		MovementMode = MovementComponent->MovementMode;

		if (MovementMode == EMovementMode::MOVE_Falling)
		{
			FallSpeed = Velocity.Z;
		}
		else
		{
			FallSpeed = 0.f;
		}
	}

	// 是否疾跑
	bIsSprinting = OwnerPawn->bIsSprinting;
}

const TEnumAsByte<EMovementDirection::Type> UTAnimInstance::ConvertDirection(float NewDirection) const
{
	if (NewDirection >= -45.f && NewDirection < 45.f)
	{
		return EMovementDirection::Fwd;
	}
	else if (NewDirection >= 45.f && NewDirection < 135.f)
	{
		return EMovementDirection::Right;
	}
	else if (NewDirection >= -135.f && NewDirection < -45.f)
	{
		return EMovementDirection::Left;
	}
	else
	{
		return EMovementDirection::Bwd;
	}
}