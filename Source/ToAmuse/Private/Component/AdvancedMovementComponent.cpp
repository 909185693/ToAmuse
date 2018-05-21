// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "AdvancedMovementComponent.h"


UAdvancedMovementComponent::UAdvancedMovementComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NavAgentProps.bCanJump = true;

	bAutoActivate = true;
	bReplicates = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAdvancedMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAdvancedMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

