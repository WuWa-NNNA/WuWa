// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/WWCharacter.h"
#include "InputActionValue.h"
#include "../../Interface/ParringTimingInterface.h"
#include "Monster.generated.h"


class UInputAction;
class USkeletalMeshComponent;
/**
 * 
 */
UCLASS()
class WUWA_API AMonster : public AWWCharacter, public IParringTimingInterface
{
	GENERATED_BODY()

public:
	AMonster(const FObjectInitializer& ObjectInitializer);
	USkeletalMeshComponent* GetWeapon() { return Weapon; }

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	bool bIsDead = false;

	void SetDead();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaSeconds) override;

private:

private:
	void BeginComboAttack();
	void SetAttackComboTimer();
	void CheckAttackComboInput();

private:
	void SetRotationByMoveInput();

public :
	void showLockOnMonster();
	void hideLockOnMonster();

protected:
	bool bIsBeingParringTiming = false;

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
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> JumpMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackMontage;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> ParryingHitEffect;

public :
	UFUNCTION(BlueprintCallable)
	void DamagedTestBoss();

	void SetIsParringTiming_Implementation(bool InIsParringTiming) override;
};
