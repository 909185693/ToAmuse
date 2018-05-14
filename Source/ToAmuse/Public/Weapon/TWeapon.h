// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "TWeapon.generated.h"

USTRUCT()
struct FAction
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float StiffTime;

	UPROPERTY()
	float ForcedTime;

	UPROPERTY()
	class UAnimMontage* Montage;

	UPROPERTY()
	class USoundCub* Sound;
};

UCLASS()
class TOAMUSE_API ATWeapon : public AActor
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void OnPressed();
	virtual void OnReleased();

protected:
	uint32 bIsPressed : 1;

	int32 ActionStage;

	float LastActionTime;

	UPROPERTY(EditDefaultsOnly, Category = Action)
	TArray<FAction> Actions;
	
	virtual void OnAction(int32 Stage);
	
	void CheckAction(bool bIsForce);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerAction(int32 Stage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAction(int32 Stage);

	virtual void ActionTick(float DeltaSeconds);

	////////////////////////////////// Common ////////////////////////////////////////
public:
	template<class T> void GetOwnerActor<T>() const
	{
		return Cast<T>(GetOwner());
	}

	const TEnumAsByte<enum ENetRole> GetOwnerRole() const;
};
