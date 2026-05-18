// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SigillumAnimInstance.generated.h"

USTRUCT(BlueprintType)
struct FParalysisState
{
	GENERATED_BODY()
	uint8 bIsParalysis : 1;
	uint32 Cycle = 3;
};


/**
 * 
 */
UCLASS()
class WUWA_API USigillumAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USigillumAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_AN_EndDiveAttack();

	UFUNCTION()
	void AnimNotify_AN_CheckParalysis();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ASigillum> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDeath : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FParalysisState ParalysisState;
};
