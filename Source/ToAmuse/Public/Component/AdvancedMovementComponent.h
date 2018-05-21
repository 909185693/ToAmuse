// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "AdvancedMovementComponent.generated.h"


UCLASS()
class TOAMUSE_API UAdvancedMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
