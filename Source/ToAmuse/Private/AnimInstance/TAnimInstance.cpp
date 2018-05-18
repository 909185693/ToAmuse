// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "TAnimInstance.h"
#include "TCharacter.h"


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

	FVector OutDir;
	Velocity.ToDirectionAndLength(OutDir, Speed);

	// 设置角色运动方向
	const FRotator ActorRotator = OwnerPawn->GetActorRotation();
	FQuat AQuat = FQuat(Velocity.Rotation());
	FQuat BQuat = FQuat(ActorRotator.GetInverse());
	const FRotator ComposeRotator = FRotator(BQuat*AQuat);

	Direction = ComposeRotator.Yaw > 180.f ? ComposeRotator.Yaw - 360.f : ComposeRotator.Yaw;
}
