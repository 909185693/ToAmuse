// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "TCharacter.h"
#include "Camera/CameraComponent.h"


FName ATCharacter::CameraSocket = TEXT("camera");

ATCharacter::ATCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	if (Camera)
	{
		Camera->bUsePawnControlRotation = false;
		Camera->SetupAttachment(GetMesh(), CameraSocket);
	}
}

void ATCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

