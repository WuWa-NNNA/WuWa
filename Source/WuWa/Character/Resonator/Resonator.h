#pragma once

#include "CoreMinimal.h"
#include "Character/WWCharacter.h"
#include "InputActionValue.h"
#include "Interface/WWCharacterWidgetInterface.h"
#include "Resonator.generated.h"

class USkeletalMeshComponent;
class UInputAction;
class UAnimMontage;
class UAttackComboData;

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
class WUWA_API AResonator : public AWWCharacter, public IWWCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	AResonator();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void TickLocomotionGait(float DeltaSeconds);

public:
	UFUNCTION(BlueprintCallable)
	virtual void ChangeState(const EResonatorState NextState);
	virtual void ChangeLocomotionGait(const ELocomotionGait NextLocomotionGait);

protected:
	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);
	virtual void Jump();
	virtual void Dash();
	virtual void Attack();
	virtual void Skill();

private:
	void SetRotationByMoveInput();
	void TryCancelAttackMontageByNewInput();
	void BeginComboAttack();
	void SetAttackComboTimer();
	void CheckAttackComboInput();

protected:
	virtual void OnDashMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	virtual void OnAttackMontageEnded(UAnimMontage* TargetMontage, bool bInterrupted);

public:
	FORCEINLINE EResonatorState GetCurrentState() const { return CurrentState; }
	FORCEINLINE ELocomotionGait GetCurrentLocomotionGait() const { return CurrentLocomotionGait; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMeshComponent() const { return Weapon; }

public:
	FORCEINLINE void SetAttackMontage(UAnimMontage* NewAttackMontage) { AttackMontage = NewAttackMontage; }
	FORCEINLINE void SetAttackComboData(UAttackComboData* NewAttackComboData) { AttackComboData = NewAttackComboData; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resonator", meta = (AllowPrivateAccess = "true"))
	EResonatorState CurrentState;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FVector CurrentMoveInputDirection;
	ELocomotionGait CurrentLocomotionGait;

private:
	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowPrivateAccess = "true"))
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
	TObjectPtr<UInputAction> SAttackAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillAction;

private:
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> JumpMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> JumpRunMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DashMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> JumpDashMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> SAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> SkillMontage;

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttackComboData> AttackComboData;

protected :	// UI + Stat
	/*UPROPERTY(VisibleAnywhere, Category = "Stat", Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UPlayerStatComponent> ResonatorStat;*/
	// -> 자식에서 stat을 다시 선언하지 않고 부모에 WWStat을 PlayerStat으로 다운캐스팅하여 사용

	UPROPERTY(VisibleAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWWWidgetComponent> HpBar;

	UPROPERTY(VisibleAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWWWidgetComponent> DashBar;

	virtual void SetupCharacterWidget(class UWWUserWidget* InUserWidget) override;
};
