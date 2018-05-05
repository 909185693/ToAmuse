// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "TCharacter.generated.h"


UCLASS()
class TOAMUSE_API ATCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
		
private:
	static FName CameraSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
};
