// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "PostProcessAnimInstance.h"
#include "TCharacter.h"


UPostProcessAnimInstance::UPostProcessAnimInstance(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bEnableFootIK = true;
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
}