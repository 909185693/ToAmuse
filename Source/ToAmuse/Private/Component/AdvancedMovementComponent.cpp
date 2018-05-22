// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "AdvancedMovementComponent.h"
#include "TCharacter.h"


UAdvancedMovementComponent::UAdvancedMovementComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NavAgentProps = FAdvancedMovementProperties();

	GetAdvancedMovementProperties().bCanJump = true;
	GetAdvancedMovementProperties().bCanSprint = true;

	MaxWalkSpeed = 480.f;
	MaxSprintSpeed = 640.f;

	JumpZVelocity = 600.f;
	BrakingDecelerationFalling = 0.2f;
	AirControl = 0.2f;
	AirControlBoostMultiplier = 5.f;

	bAutoActivate = true;
	bReplicates = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UAdvancedMovementComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UAdvancedMovementComponent::PostLoad()
{
	Super::PostLoad();

	TCharacterOwner = Cast<ATCharacter>(PawnOwner);
}

void UAdvancedMovementComponent::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	Super::SetUpdatedComponent(NewUpdatedComponent);

	TCharacterOwner = Cast<ATCharacter>(PawnOwner);
}

void UAdvancedMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UAdvancedMovementComponent::GetMaxSpeed() const
{
	switch (MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
		return IsSprinting() ? MaxSprintSpeed : (IsCrouching() ? MaxWalkSpeedCrouched : MaxWalkSpeed);
	case MOVE_Falling:
		return IsSprinting() ? MaxSprintSpeed : MaxWalkSpeed;
	case MOVE_Swimming:
		return MaxSwimSpeed;
	case MOVE_Flying:
		return MaxFlySpeed;
	case MOVE_Custom:
		return MaxCustomMovementSpeed;
	case MOVE_None:
	default:
		return 0.f;
	}
}

bool UAdvancedMovementComponent::IsSprinting() const
{
	return TCharacterOwner && TCharacterOwner->bIsSprinting;
}

void UAdvancedMovementComponent::Sprint(bool bClientSimulation)
{
	if (!HasValidData())
	{
		return;
	}

	if (!bClientSimulation && !CanSprintInCurrentState())
	{
		return;
	}

	if (!bClientSimulation)
	{
		TCharacterOwner->bIsSprinting = true;
	}
	TCharacterOwner->OnStartSprint();
}

void UAdvancedMovementComponent::UnSprint(bool bClientSimulation)
{
	if (!HasValidData())
	{
		return;
	}

	if (!bClientSimulation)
	{
		TCharacterOwner->bIsSprinting = false;
	}
	TCharacterOwner->OnEndSprint();
}

bool UAdvancedMovementComponent::CanSprintInCurrentState() const
{
	if (!CanEverSrpint())
	{
		return false;
	}

	return IsFalling() || IsMovingOnGround();
}

void UAdvancedMovementComponent::UpdateCharacterStateBeforeMovement()
{
	// Check for a change in crouch state. Players toggle crouch by changing bWantsToCrouch.
	const bool bAllowedToCrouch = CanCrouchInCurrentState();
	if ((!bAllowedToCrouch || !bWantsToCrouch || bWantsToSprint) && IsCrouching())
	{
		UnCrouch(false);
	}
	else if (bWantsToCrouch && !bWantsToSprint && bAllowedToCrouch && !IsCrouching())
	{
		Crouch(false);
	}

	// Check for a change in crouch state. Players toggle crouch by changing bWantsToSprint.
	const bool bAllowedToSprint = CanSprintInCurrentState();
	if ((!bAllowedToSprint || !bWantsToSprint) && IsSprinting())
	{
		UnSprint(false);
	}
	else if (bWantsToSprint && bAllowedToSprint && !IsSprinting())
	{
		Sprint(false);
	}
}

void UAdvancedMovementComponent::UpdateCharacterStateAfterMovement()
{
	Super::UpdateCharacterStateAfterMovement();

	// Unsprint if no longer allowed to be crouched
	if (IsSprinting() && !CanSprintInCurrentState())
	{
		UnSprint(false);
	}
}

struct FAdvancedMovementProperties& UAdvancedMovementComponent::GetAdvancedMovementProperties() const
{
	return (struct FAdvancedMovementProperties&)static_cast<const FAdvancedMovementProperties&>(NavAgentProps);
}

