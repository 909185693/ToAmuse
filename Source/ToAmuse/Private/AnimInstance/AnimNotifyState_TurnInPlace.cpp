// Copyright 2018 by January. All Rights Reserved.

#include "AnimNotifyState_TurnInPlace.h"
#include "TAnimInstance.h"


UAnimNotifyState_TurnInPlace::UAnimNotifyState_TurnInPlace(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UAnimNotifyState_TurnInPlace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	UTAnimInstance* AnimInstance = MeshComp ? Cast<UTAnimInstance>(MeshComp->GetAnimInstance()) : nullptr;
	if (AnimInstance != nullptr)
	{
		AnimInstance->StartTurnInPlace();
	}

	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UAnimNotifyState_TurnInPlace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	UTAnimInstance* AnimInstance = MeshComp ? Cast<UTAnimInstance>(MeshComp->GetAnimInstance()) : nullptr;
	if (AnimInstance != nullptr)
	{
		AnimInstance->TurnInPlace(FrameDeltaTime);
	}

	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimNotifyState_TurnInPlace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UTAnimInstance* AnimInstance = MeshComp ? Cast<UTAnimInstance>(MeshComp->GetAnimInstance()) : nullptr;
	if (AnimInstance != nullptr)
	{
		AnimInstance->StopTurnInPlace();
	}

	Super::NotifyEnd(MeshComp, Animation);
}
