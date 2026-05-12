#pragma once

#include "CoreMinimal.h"
#include "Character/WWCharacter.h"
#include "InputActionValue.h"
#include "Resonator.generated.h"

class UInputAction;
class USkeletalMeshComponent;

UENUM(BlueprintType)
enum class EResonatorState : uint8
{
	Normal,
	Attack
};

UENUM(BlueprintType)
enum class ELocomotionGait : uint8
{
	Run,
	Dash,
	Sprint
};

UCLASS()
class WUWA_API AResonator : public AWWCharacter
{
	GENERATED_BODY()

public:
	AResonator();

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetCurrentState(const EResonatorState NextState);
	virtual void SetCurrentLocomotionGait(const ELocomotionGait NextLocomotionGait);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	virtual void Tick(float DeltaSeconds) override;
	virtual void TickLocomotionGait(float DeltaSeconds);

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void Dash();
	void Attack();
	void Skill();

private:
	void SetRotationByMoveInput();
	void TryCancelAttackByNewInput();
	void BeginComboAttack();
	void SetAttackComboTimer();
	void CheckAttackComboInput();

private:
	void OnDashMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnAttackMontageEnded(UAnimMontage* TargetMontage, bool bInterrupted);
	void OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	FORCEINLINE EResonatorState GetCurrentState() const { return CurrentState; }
	FORCEINLINE ELocomotionGait GetCurrentLocomotionGait() const { return CurrentLocomotionGait; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMeshComponent() const { return Weapon; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resonator", meta = (AllowPrivateAccess = "true"))
	EResonatorState CurrentState;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FVector CurrentMoveInputDirection;
	ELocomotionGait CurrentLocomotionGait;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool bApplyZMotionToCamera = false;

private:
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
	TObjectPtr<UInputAction> DashAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillAction;

private:
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> JumpMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> JumpRunMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DashMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> JumpDashMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SkillMontage;

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAttackComboData> AttackComboData;
};
