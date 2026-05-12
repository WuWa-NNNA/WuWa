// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/WWCharacter.h"
#include "InputActionValue.h"
#include "Monster.generated.h"


class UInputAction;

/**
 * 
 */
UCLASS()
class WUWA_API AMonster : public AWWCharacter
{
	GENERATED_BODY()

public:
	AMonster();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Attack();


private:
	virtual void Tick(float DeltaSeconds) override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();

private:

private:
	void BeginComboAttack();
	void EndComboAttack(UAnimMontage* TargetMontage, bool bInterrupted);
	void SetAttackComboTimer();
	void CheckAttackComboInput();

private:
	void SetRotationByMoveInput();


private:
	UPROPERTY(BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	FVector CurrentMoveInputDirection;

private:
	uint32 CurrentAttackCombo = 0;
	bool bHasNextComboCommand = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	bool bCanCancelAttack = false;
	FTimerHandle AttackComboTimer;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

private:
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;

private:
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> JumpMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackMontage;
};
