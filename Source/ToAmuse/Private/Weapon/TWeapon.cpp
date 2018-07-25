// Copyright 2018 by January. All Rights Reserved.

#include "TWeapon.h"
#include "TGameplayStatics.h"
#include "GameFramework/Character.h"


ATWeapon::ATWeapon(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsPressed = false;
	LastActionTime = 0.f;
	ActionStage = 0;
}

void ATWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ActionTick(DeltaSeconds);
}

void ATWeapon::OnPressed()
{
	bIsPressed = true;

	CheckAction(true);
}

void ATWeapon::OnReleased()
{
	bIsPressed = false;
}

void ATWeapon::OnAction(int32 Stage)
{
	if (Role == ROLE_AutonomousProxy)
	{
		ServerAction(Stage);
	}

	LastActionTime = GetWorld()->TimeSeconds;

	class ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character != nullptr)
	{
		const FAction& Action = Actions[Stage];

		float ActionSpeed = 1.f;

		Character->PlayAnimMontage(Action.Montage, ActionSpeed);

		UTGameplayStatics::PlaySoundAtLocation(GetWorld(), Action.Sound, GetActorLocation());
	}
}

bool ATWeapon::ServerAction_Validate(int32 Stage)
{
	return true;
}

void ATWeapon::ServerAction_Implementation(int32 Stage)
{
	MulticastAction(Stage);
}

void ATWeapon::MulticastAction_Implementation(int32 Stage)
{
	if (GetOwnerRole() != ROLE_AutonomousProxy)
	{
		OnAction(Stage);
	}
}

void ATWeapon::ActionTick(float DeltaSeconds)
{
	class ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && Character->IsLocallyControlled())
	{
		CheckAction(false);
	}
}

void ATWeapon::CheckAction(bool bIsForce)
{
	if (Actions.IsValidIndex(ActionStage))
	{
		const FAction& Action = Actions[ActionStage];

		const float TimeSeconds = GetWorld()->TimeSeconds;
		const float CheckTime = bIsForce ? Action.ForcedTime : Action.StiffTime;
		if (TimeSeconds - LastActionTime >= CheckTime)
		{
			OnAction(ActionStage);
			
			ActionStage = (++ActionStage % Actions.Num());
		}
	}
}

const TEnumAsByte<enum ENetRole> ATWeapon::GetOwnerRole() const
{
	return GetOwner() ? GetOwner()->Role : TEnumAsByte<enum ENetRole>(ROLE_None);
}