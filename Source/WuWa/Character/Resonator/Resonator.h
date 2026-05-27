#pragma once

#include "CoreMinimal.h"
#include "Character/WWCharacter.h"
#include "InputActionValue.h"
#include "Resonator.generated.h"

class USkeletalMeshComponent;
class UInputAction;
class UAnimMontage;
class UAttackComboData;

UENUM(BlueprintType)
enum class EResonatorState : uint8
{
	Normal,
	Attack,
	Hit,
	Dodge
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
	AResonator(const FObjectInitializer& ObjectInitializer);

public:
	bool CanConcerto();
	void ConcertoOut();
	void ConcertoIn(AResonator* Other);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void InitializeCinematicActors();
	void InitializeUIComponents();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void TickCamera(float DeltaSeconds);
	virtual void TickNormal(float DeltaSeconds);
	virtual void TickAttack(float DeltaSeconds);
	virtual void TickLocomotionGait(float DeltaSeconds);
	virtual void TickUIWidget(float DeltaSeconds);

public:
	UFUNCTION(BlueprintCallable)
	virtual void ChangeState(const EResonatorState NextState);
	virtual void ChangeLocomotionGait(const ELocomotionGait NextLocomotionGait);

public:
	void DeactivateByConcerto();
	void SetCameraLag(bool bNewValue);

protected:
	virtual void Move(const FInputActionValue& Value);
	virtual void StopMove(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);
	virtual void Lock();
	virtual void Jump();
	virtual void Dash();
	virtual void Dodge();
	virtual void Attack();
	virtual void Skill();
	virtual void Burst();
	virtual void ConcertoAttack();

protected:
	virtual void OnAttackSucceeded(TSet<TObjectPtr<AActor>>& DamagedActors, AActor* HitActor, const FHitResult& HitResult, bool& bDidShakeCamera) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void ProcessAttack();
	virtual bool CanAirDash();
	virtual void PlayDashMontage();
	virtual void PlayDodgeMontage();
	virtual void PlayBurstCinematic();

private:
	void SetRotationByMoveInput();
	void SetAttackRotationByMoveInput();
	void TryCancelAttackMontageByNewInput();
	void OnFinishedDodgeTimer();
	void SpawnGhostTrailEffect();
	void BeginComboAttack();
	void SetAttackComboTimer();
	void CheckAttackComboInput();
	void BeginConcertoGhostTrailEffect();
	void OnConcertoBlendEnded();

public:
	virtual void OnDashMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	virtual void OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	virtual void OnAttackMontageEnded(UAnimMontage* TargetMontage, bool bInterrupted);
	UFUNCTION()
	virtual void OnBurstCinematicEnded();

public:
	FORCEINLINE EResonatorState GetCurrentState() const { return CurrentState; }
	FORCEINLINE ELocomotionGait GetCurrentLocomotionGait() const { return CurrentLocomotionGait; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMeshComponent() const { return Weapon; }
	FORCEINLINE UAnimMontage* GetDashMontage() const { return DashMontage; }
	FORCEINLINE UAnimMontage* GetDodgeMontage() const { return DodgeMontage; }
	FORCEINLINE UAnimMontage* GetWeaponDashMontage() const { return WeaponDashMontage; }
	FORCEINLINE bool HasCurrentMoveInput() const { return bHasCurrentMoveInput; }
	FORCEINLINE FVector GetCurrentMoveInputDirection() const { return CurrentMoveInputDirection; }
	FORCEINLINE uint32 GetCurrentAttackCombo() const { return CurrentAttackCombo; }
	FORCEINLINE AActor* GetLockOnTarget() const { return LockOnTarget; }
	UFUNCTION(BlueprintCallable)
	virtual FRotator GetLockOnRotator() const { return (LockOnTarget->GetActorLocation() - GetActorLocation()).Rotation(); }

public:
	FORCEINLINE void SetDashMontage(UAnimMontage* NewDashMontage) { DashMontage = NewDashMontage; }
	FORCEINLINE void SetDodgeMontage(UAnimMontage* NewDodgeMontage) { DodgeMontage = NewDodgeMontage; }
	FORCEINLINE void SetAttackMontage(UAnimMontage* NewAttackMontage) { AttackMontage = NewAttackMontage; }
	FORCEINLINE void SetWeaponAttackMontage(UAnimMontage* NewWeaponAttackMontage) { WeaponAttackMontage = NewWeaponAttackMontage; }
	FORCEINLINE void SetAttackComboData(UAttackComboData* NewAttackComboData) { AttackComboData = NewAttackComboData; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resonator", meta = (AllowPrivateAccess = "true"))
	EResonatorState CurrentState;

private:
	bool bHasCurrentMoveInput = false;

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
	bool bHasDashedInAir = false;
	bool bHasCurrentDashInput = false;
	float DodgeTime = 0.0f;
	FTimerHandle DodgeTimer;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool bApplyZMotionToCamera = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool bIsLockOn = false;

	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> LockOnTarget = nullptr;

private:
	UPROPERTY(EditAnywhere, Category = "Cinematic", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> CineRoot;

	UPROPERTY(EditAnywhere, Category = "Cinematic", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ALevelSequenceActor> SequenceActor;

	UPROPERTY(EditAnywhere, Category = "Cinematic", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ACineCameraActor> CineCameraActor;

	UPROPERTY(EditAnywhere, Category = "Cinematic", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> CineLookAtActor;

	UPROPERTY(EditAnywhere, Category = "Cinematic", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULevelSequence> BurstSequence;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Weapon;

private:
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LockAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DashAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> BurstAction;

private:
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> JumpMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> JumpRunMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DashMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> JumpDashMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> BurstMontage;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ConcertoAttackMontage;

private:
	UPROPERTY(EditAnywhere, Category = "WeaponMontage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> WeaponDashMontage;

	UPROPERTY(EditAnywhere, Category = "WeaponMontage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> WeaponAttackMontage;

	UPROPERTY(EditAnywhere, Category = "WeaponMontage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> WeaponSkillMontage;

	UPROPERTY(EditAnywhere, Category = "WeaponMontage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> WeaponBurstMontage;

	UPROPERTY(EditAnywhere, Category = "WeaponMontage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> WeaponConcertoAttackMontage;

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttackComboData> AttackComboData;

private:
	UPROPERTY(EditAnywhere, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AGhostTrailEffect> GhostTrailEffectClass;

	UPROPERTY(EditAnywhere, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMaterialInterface> GhostTrailEffectMaterial;

	FTimerHandle GhostTrailEffectSpawnTimer;

	FTimerHandle ConcertoBlendTimer;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UWidgetComponent* DashGaugeComponent;

private :
	FVector CurrentLocation;
	FVector TargetLocation;
	FVector NewLocation;

public :
	UFUNCTION(BlueprintCallable, Category = "test")
	void DamagedTest();
	UFUNCTION(BlueprintCallable, Category = "test")
	void RGaugeUp();
	UFUNCTION(BlueprintCallable, Category = "test")
	void OpenUI();
	UFUNCTION(BlueprintCallable, Category = "test")
	void CloseUI();

	UFUNCTION(BlueprintCallable, Category = "test")
	void CloseBossUI();

private:
	void UseDashGauge();
	void UpdateDashGaugeUI(float NewDash);
};
