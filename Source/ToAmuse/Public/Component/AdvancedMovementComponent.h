// Copyright 2018 by NiHongjian. All Rights Reserved.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "AdvancedMovementComponent.generated.h"


class ATCharacter;

USTRUCT(BlueprintType)
struct FAdvancedMovementProperties : public FNavAgentProperties
{
	GENERATED_USTRUCT_BODY()

	/** If true, this Pawn is capable of sprinting. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementProperties)
	uint32 bCanSprint : 1;

	FAdvancedMovementProperties()
		: FNavAgentProperties()
		, bCanSprint(false)
	{

	}
};

UCLASS()
class TOAMUSE_API UAdvancedMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void PostLoad() override;
	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;

public:
	//BEGIN UAdvancedMovementComponent Interface
	virtual float GetMaxSpeed() const override;
	virtual bool IsSprinting() const;
	//BEGIN UAdvancedMovementComponent Interface

protected:
	/** TCharacter movement component belongs to */
	UPROPERTY(Transient, DuplicateTransient)
	ATCharacter* TCharacterOwner;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/** The maximum ground speed when Sprinting. Also determines maximum lateral speed when falling. */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float MaxSprintSpeed;

	/** If true, try to crouch (or keep crouching) on next update. If false, try to stop crouching on next update. */
	UPROPERTY(Category = "Character Movement (General Settings)", VisibleInstanceOnly, BlueprintReadOnly)
	uint32 bWantsToSprint : 1;

	/** Return advanced movement properties  */
	struct FAdvancedMovementProperties& GetAdvancedMovementProperties() const;

	/** @return true if component can crouch */
	FORCEINLINE bool CanEverSrpint() const { return GetAdvancedMovementProperties().bCanSprint; }

	/**
	* Checks if new capsule size fits (no encroachment), and call CharacterOwner->OnStartCrouch() if successful.
	* In general you should set bWantsToSprint instead to have the srpint persist during movement, or just use the srpint functions on the owning Character.
	* @param	bClientSimulation	true when called when bIsSrpinting is replicated to non owned clients, to update collision cylinder and offset.
	*/
	virtual void Sprint(bool bClientSimulation = false);

	/**
	* Checks if default capsule size fits (no encroachment), and trigger OnEndCrouch() on the owner if successful.
	* @param	bClientSimulation	true when called when bIsCrouched is replicated to non owned clients, to update collision cylinder and offset.
	*/
	virtual void UnSprint(bool bClientSimulation = false);

	/** @return true if the character is allowed to crouch in the current state. By default it is allowed when walking or falling, if CanEverCrouch() is true. */
	virtual bool CanSprintInCurrentState() const;

	/** Update the character state in PerformMovement right before doing the actual position change */
	virtual void UpdateCharacterStateBeforeMovement() override;

	/** Update the character state in PerformMovement after the position change. Some rotation updates happen after this. */
	virtual void UpdateCharacterStateAfterMovement();
};
