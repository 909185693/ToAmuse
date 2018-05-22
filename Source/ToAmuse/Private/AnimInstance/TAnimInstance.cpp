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
	const FRotator ActorRotator = OwnerPawn->GetActorRotation();
	FQuat AQuat = FQuat(Velocity.Rotation());
	FQuat BQuat = FQuat(ActorRotator.GetInverse());
	const FRotator ComposeRotator = FRotator(BQuat*AQuat);

	Direction = ComposeRotator.Yaw > 180.f ? ComposeRotator.Yaw - 360.f : ComposeRotator.Yaw;

	// 旋转速度
	RotationSpeed = OwnerPawn->RotationSpeed;

	// 移动模式
	class UCharacterMovementComponent* MovementComponent = OwnerPawn ? OwnerPawn->GetCharacterMovement() : nullptr;
	if (MovementComponent != nullptr)
	{
		MovementMode = MovementComponent->MovementMode;
		//bIsJumping = OwnerPawn->IsJumpProvidingForce()

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
