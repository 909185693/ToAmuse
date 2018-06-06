// Copyright 2018 by NiHongjian. All Rights Reserved.

#include "TCharacter.h"
#include "ToAmuse.h"
#include "Camera/CameraComponent.h"
#include "AdvancedMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


ATCharacter::ATCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UAdvancedMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;

	GetCapsuleComponent()->InitCapsuleSize(90.f, 30.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	if (CameraBoom)
	{
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->TargetArmLength = 300.0f;
		CameraBoom->bUsePawnControlRotation = true;
	}

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	if (FollowCamera)
	{
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
		FollowCamera->bUsePawnControlRotation = false;
	}
	
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

void ATCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FRotator ControllerRotation = GetControlRotation();
	//FRotator PawnCurrentRotation = GetActorRotation();

	//ControllerRotation.Pitch = 0.f;
	//ControllerRotation.Roll = 0.f;

	//PawnCurrentRotation.Pitch = 0.f;
	//PawnCurrentRotation.Roll = 0.f;

	//const FRotator& PawnTargetRotation = FMath::RInterpConstantTo(PawnCurrentRotation, ControllerRotation, DeltaTime, RotationInterpSpeed);

	//OnScreenDebugMessage(1, FString::Printf(TEXT("PawnTargetRotation [%s]"), *PawnTargetRotation.ToCompactString()));
}

void ATCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ATCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ATCharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ATCharacter::StopJumping);

	InputComponent->BindAction<FOnSprintAction, ATCharacter, bool>("Sprint", IE_Pressed, this, &ATCharacter::Sprint, false);
	InputComponent->BindAction<FOnSprintAction, ATCharacter, bool>("Sprint", IE_Released, this, &ATCharacter::UnSprint, false);

	InputComponent->BindAxis("MoveForward", this, &ATCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ATCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ATCharacter::LookUpAtRate);
}

void ATCharacter::MoveForward(float Value)
{
	ForwardAxisValue = Value;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATCharacter::MoveRight(float Value)
{
	RightAxisValue = Value;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATCharacter::Sprint(bool bClientSimulation)
{
	class UAdvancedMovementComponent* AdvancedMovement = Cast<UAdvancedMovementComponent>(GetCharacterMovement());
	if (AdvancedMovement)
	{
		if (CanSprint())
		{
			AdvancedMovement->bWantsToSprint = true;
		}
	}
}

void ATCharacter::UnSprint(bool bClientSimulation)
{
	class UAdvancedMovementComponent* AdvancedMovement = Cast<UAdvancedMovementComponent>(GetCharacterMovement());
	if (AdvancedMovement)
	{
		AdvancedMovement->bWantsToSprint = false;
	}
}

bool ATCharacter::CanSprint()
{
	class UAdvancedMovementComponent* AdvancedMovement = Cast<UAdvancedMovementComponent>(GetCharacterMovement());
	return !bIsSprinting && AdvancedMovement && AdvancedMovement->CanEverSrpint() && GetRootComponent() && !GetRootComponent()->IsSimulatingPhysics();
}

void ATCharacter::OnEndSprint()
{

}

void ATCharacter::OnStartSprint()
{

}

void ATCharacter::OnRep_IsSprinting()
{
	class UAdvancedMovementComponent* AdvancedMovement = Cast<UAdvancedMovementComponent>(GetCharacterMovement());
	if (AdvancedMovement)
	{
		if (bIsSprinting)
		{
			AdvancedMovement->Sprint(true);
		}
		else
		{
			AdvancedMovement->UnSprint(true);
		}
	}
}

void ATCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ATCharacter, bIsSprinting, COND_SimulatedOnly);
}