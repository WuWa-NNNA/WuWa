// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/WWCharacter.h"
#include "InputActionValue.h"
#include "Monster.generated.h"


class UInputAction;
class USkeletalMeshComponent;
/**
 * 
 */
UCLASS()
class WUWA_API AMonster : public AWWCharacter
{
	GENERATED_BODY()

public:
	AMonster(const FObjectInitializer& ObjectInitializer);
	USkeletalMeshComponent* GetWeapon() { return Weapon; }

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Attack();
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaSeconds) override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();

private:

private:
	void BeginComboAttack();
	void SetAttackComboTimer();
	void CheckAttackComboInput();

private:
	void SetRotationByMoveInput();

public :
	void showLockOn();
	void hideLockOn();

private:
	UPROPERTY(BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	FVector CurrentMoveInputDirection;

private:
	uint32 CurrentAttackCombo = 0;
	bool bHasNextComboCommand = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	bool bCanCancelAttack = false;
	FTimerHandle AttackComboTimer;

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Weapon;

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


public :
	UFUNCTION(BlueprintCallable)
	void DamagedTestBoss();

	UFUNCTION(BlueprintCallable)
	void PlayDamagedSkin(float damage);

};
