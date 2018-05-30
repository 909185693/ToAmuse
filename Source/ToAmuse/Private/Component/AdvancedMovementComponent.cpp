// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "AdvancedMovementComponent.h"
#include "TCharacter.h"
#include "ToAmuse.h"

DECLARE_CYCLE_STAT(TEXT("Char ClientUpdatePositionAfterServerUpdate"), STAT_CharacterMovementClientUpdatePositionAfterServerUpdate, STATGROUP_Character);

UAdvancedMovementComponent::UAdvancedMovementComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NavAgentProps = FAdvancedMovementProperties();

	GetAdvancedMovementProperties().bCanJump = true;
	GetAdvancedMovementProperties().bCanSprint = true;

	MaxWalkSpeed = 800.f;
	MaxSprintSpeed = 1000.f;

	JumpZVelocity = 800.f;
	MaxAcceleration = 256.f;
	BrakingFrictionFactor = 1.f;
	GroundFriction = 1.0f;
	BrakingDecelerationWalking = 128.f;
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

	if (TCharacterOwner && TCharacterOwner->IsLocallyControlled())
	{
		const FVector LastInputVector = GetLastInputVector();
		if (LastInputVector.IsNearlyZero(1.0e-03f))
		{
			FRotator ControlRotation = TCharacterOwner->GetControlRotation();

			ControlRotation.Pitch = 0.f;
			ControlRotation.Roll = 0.f;

			MovementInputVector = ControlRotation.Vector() * TCharacterOwner->ForwardAxisValue + ControlRotation.Vector() * TCharacterOwner->RightAxisValue;
		}
		else
		{
			MovementInputVector = LastInputVector;
		}

		SyncMovementInputVector(MovementInputVector);

		OnScreenDebugMessage(1, FString::Printf(TEXT("1.MovementInputVector[%s] ForwardAxisValue[%.2f] RightAxisValue[%0.2f]"), *MovementInputVector.ToCompactString(), TCharacterOwner->ForwardAxisValue, TCharacterOwner->RightAxisValue));
	}
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

void UAdvancedMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	PrevMovementMode = PreviousMovementMode;
	PrevCustomMovementMode = PreviousCustomMode;
}

void UAdvancedMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	if (!CharacterOwner)
	{
		return;
	}

	const bool bWasJumping = CharacterOwner->bPressedJump;

	CharacterOwner->bPressedJump = ((Flags & FSavedMove_Character::FLAG_JumpPressed) != 0);
	bWantsToCrouch = ((Flags & FSavedMove_Character::FLAG_WantsToCrouch) != 0);
	bWantsToSprint = ((Flags & FLAG_WantsToSprint) != 0);

	// Reset JumpKeyHoldTime when player presses Jump key on server as well.
	if (!bWasJumping && CharacterOwner->bPressedJump)
	{
		CharacterOwner->bWasJumping = false;
		CharacterOwner->JumpKeyHoldTime = 0.0f;
	}
}

bool UAdvancedMovementComponent::ClientUpdatePositionAfterServerUpdate()
{
	SCOPE_CYCLE_COUNTER(STAT_CharacterMovementClientUpdatePositionAfterServerUpdate);
	if (!HasValidData())
	{
		return false;
	}

	FNetworkPredictionData_Client_Character* ClientData = GetPredictionData_Client_Character();
	check(ClientData);

	if (!ClientData->bUpdatePosition)
	{
		return false;
	}

	if (bIgnoreClientMovementErrorChecksAndCorrection)
	{
		//#if !UE_BUILD_SHIPPING
		//		if (CharacterMovementCVars::NetShowCorrections != 0)
		//		{
		//			UE_LOG(LogNetPlayerMovement, Warning, TEXT("*** Client: %s is set to ignore error checks and corrections with %d saved moves in queue."), *GetNameSafe(CharacterOwner), ClientData->SavedMoves.Num());
		//		}
		//#endif // !UE_BUILD_SHIPPING
		return false;
	}

	ClientData->bUpdatePosition = false;

	// Don't do any network position updates on things running PHYS_RigidBody
	if (CharacterOwner->GetRootComponent() && CharacterOwner->GetRootComponent()->IsSimulatingPhysics())
	{
		return false;
	}

	if (ClientData->SavedMoves.Num() == 0)
	{
		UE_LOG(LogNetPlayerMovement, VeryVerbose, TEXT("ClientUpdatePositionAfterServerUpdate No saved moves to replay"), ClientData->SavedMoves.Num());

		// With no saved moves to resimulate, the move the server updated us with is the last move we've done, no resimulation needed.
		CharacterOwner->bClientResimulateRootMotion = false;
		if (CharacterOwner->bClientResimulateRootMotionSources)
		{
			// With no resimulation, we just update our current root motion to what the server sent us
			UE_LOG(LogRootMotion, VeryVerbose, TEXT("CurrentRootMotion getting updated to ServerUpdate state: %s"), *CharacterOwner->GetName());
			CurrentRootMotion.UpdateStateFrom(CharacterOwner->SavedRootMotion);
			CharacterOwner->bClientResimulateRootMotionSources = false;
		}

		return false;
	}

	// Save important values that might get affected by the replay.
	const float SavedAnalogInputModifier = AnalogInputModifier;
	const FRootMotionMovementParams BackupRootMotionParams = RootMotionParams; // For animation root motion
	const FRootMotionSourceGroup BackupRootMotion = CurrentRootMotion;
	const bool bRealJump = CharacterOwner->bPressedJump;
	const bool bRealCrouch = bWantsToCrouch;
	const bool bRealSprint = bWantsToSprint;
	const bool bRealForceMaxAccel = bForceMaxAccel;
	CharacterOwner->bClientWasFalling = (MovementMode == MOVE_Falling);
	CharacterOwner->bClientUpdating = true;
	bForceNextFloorCheck = true;

	// Replay moves that have not yet been acked.
	UE_LOG(LogNetPlayerMovement, VeryVerbose, TEXT("ClientUpdatePositionAfterServerUpdate Replaying %d Moves, starting at Timestamp %f"), ClientData->SavedMoves.Num(), ClientData->SavedMoves[0]->TimeStamp);
	for (int32 i = 0; i < ClientData->SavedMoves.Num(); i++)
	{
		const FSavedMovePtr& CurrentMove = ClientData->SavedMoves[i];
		CurrentMove->PrepMoveFor(CharacterOwner);
		MoveAutonomous(CurrentMove->TimeStamp, CurrentMove->DeltaTime, CurrentMove->GetCompressedFlags(), CurrentMove->Acceleration);
		CurrentMove->PostUpdate(CharacterOwner, FSavedMove_Character::PostUpdate_Replay);
	}

	if (ClientData->PendingMove.IsValid())
	{
		ClientData->PendingMove->bForceNoCombine = true;
	}

	// Restore saved values.
	AnalogInputModifier = SavedAnalogInputModifier;
	RootMotionParams = BackupRootMotionParams;
	CurrentRootMotion = BackupRootMotion;
	if (CharacterOwner->bClientResimulateRootMotionSources)
	{
		// If we were resimulating root motion sources, it's because we had mismatched state
		// with the server - we just resimulated our SavedMoves and now need to restore
		// CurrentRootMotion with the latest "good state"
		UE_LOG(LogRootMotion, VeryVerbose, TEXT("CurrentRootMotion getting updated after ServerUpdate replays: %s"), *CharacterOwner->GetName());
		CurrentRootMotion.UpdateStateFrom(CharacterOwner->SavedRootMotion);
		CharacterOwner->bClientResimulateRootMotionSources = false;
	}
	CharacterOwner->SavedRootMotion.Clear();
	CharacterOwner->bClientResimulateRootMotion = false;
	CharacterOwner->bClientUpdating = false;
	CharacterOwner->bPressedJump = bRealJump;
	bWantsToCrouch = bRealCrouch;
	bWantsToSprint = bRealSprint;
	bForceMaxAccel = bRealForceMaxAccel;
	bForceNextFloorCheck = true;

	return (ClientData->SavedMoves.Num() > 0);
}

FNetworkPredictionData_Client* UAdvancedMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		UAdvancedMovementComponent* MutableThis = const_cast<UAdvancedMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_TCharacter(*this);
	}

	return ClientPredictionData;
}

void UAdvancedMovementComponent::ToRagdoll()
{

}

void UAdvancedMovementComponent::UnRagdoll()
{

}

bool UAdvancedMovementComponent::SyncMovementInputVector_Validate(const FVector& NewMovementInputVector)
{
	return true;
}

void UAdvancedMovementComponent::SyncMovementInputVector_Implementation(const FVector& NewMovementInputVector)
{
	MovementInputVector = NewMovementInputVector;
}

void UAdvancedMovementComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION(UAdvancedMovementComponent, MovementInputVector, COND_SkipOwner);
}

////////////////////////////////// FSavedMove_TCharacter Begin ////////////////////////////////////////

void FSavedMove_TCharacter::Clear()
{
	FSavedMove_Character::Clear();

	bWantsToSprint = false;
}

bool FSavedMove_TCharacter::IsImportantMove(const FSavedMovePtr& LastAckedMove) const
{
	FSavedMove_TCharacter* LastTCharacterAckedMove = (FSavedMove_TCharacter*)LastAckedMove.Get();
	if (LastTCharacterAckedMove && bWantsToSprint != LastTCharacterAckedMove->bWantsToSprint)
	{
		return true;
	}

	return FSavedMove_Character::IsImportantMove(LastAckedMove);
}

bool FSavedMove_TCharacter::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	FSavedMove_TCharacter* NewTCharacterMove = (FSavedMove_TCharacter*)NewMove.Get();
	if (NewTCharacterMove && bWantsToSprint != NewTCharacterMove->bWantsToSprint)
	{
		return false;
	}

	return FSavedMove_Character::CanCombineWith(NewMove, Character, MaxDelta);
}

void FSavedMove_TCharacter::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData)
{
	FSavedMove_Character::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UAdvancedMovementComponent* AdvancedMovement = Cast<UAdvancedMovementComponent>(Character->GetCharacterMovement());
	if (AdvancedMovement != nullptr)
	{
		bWantsToSprint = AdvancedMovement->bWantsToSprint;
	}
}

uint8 FSavedMove_TCharacter::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();

	if (bWantsToSprint)
	{
		Result |= FLAG_WantsToSprint;
	}

	return Result;
}

////////////////////////////////// FSavedMove_TCharacter End ////////////////////////////////////////


////////////////////////////////// FNetworkPredictionData_Client_TCharacter Begin ////////////////////////////////////////

FNetworkPredictionData_Client_TCharacter::FNetworkPredictionData_Client_TCharacter(const UAdvancedMovementComponent& ClientMovement)
	: FNetworkPredictionData_Client_Character(ClientMovement)
{

}

FSavedMovePtr FNetworkPredictionData_Client_TCharacter::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_TCharacter());
}

////////////////////////////////// FNetworkPredictionData_Client_TCharacter End ////////////////////////////////////////