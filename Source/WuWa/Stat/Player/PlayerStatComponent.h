// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Stat/WWStatComponent.h"
#include "PlayerStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillEStartDelegate, float);
DECLARE_MULTICAST_DELEGATE(FOnSkillEStartDelegate2);


DECLARE_MULTICAST_DELEGATE_OneParam(FOnDashChangedDelegate, float /*currentDash*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBaseSkillchangeDelegate,class UPaperSprite* /*skillIcon*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRGaugaChangedDelegate, float /*RGuage*/);

DECLARE_MULTICAST_DELEGATE(FOnRSartDelegate);
DECLARE_MULTICAST_DELEGATE(FOnREndDelegate);

DECLARE_MULTICAST_DELEGATE(FOnLockOnDelegate);
DECLARE_MULTICAST_DELEGATE(FOnPlayIconAnimationDelegate);
DECLARE_MULTICAST_DELEGATE(FOnPlayIcon4AnimationDelegate);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPartyChangedDelegate, int32 /*PartyNumber*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class WUWA_API UPlayerStatComponent : public UWWStatComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public :
	FOnSkillEStartDelegate FOnSkillEStart;
	FOnSkillEStartDelegate2 OnSkillEStart2;

	//FOnSkillRStartDelegate FOnSkillRStart;
	FOnDashChangedDelegate OnDashChanged;

	FOnBaseSkillchangeDelegate OnBaseSkillchange;
	FOnRGaugaChangedDelegate OnRGaugaChanged;

	FOnRSartDelegate OnRSart;
	FOnREndDelegate OnREnd;

	FOnLockOnDelegate OnLockOn;
	FOnPlayIconAnimationDelegate OnPlayIconAnimation;
	FOnPlayIcon4AnimationDelegate OnPlayIcon4Animation;

	FOnPartyChangedDelegate OnPartyChanged;

	FORCEINLINE float GetMaxDash() { return MaxDash; }
	FORCEINLINE float GetLevel() { return Level; }
	FORCEINLINE float GetCurrentDash() { return CurrentDash; }
	FORCEINLINE void SetCurrentDash(float change) { CurrentDash = change; }
	
	FORCEINLINE float GetCoolTime_E() { return coolTime_E; }
	FORCEINLINE void SetCoolTime_E(float change) { coolTime_E = change; }

	FORCEINLINE float GetRGauge() { return RGauge; }
	FORCEINLINE void SetRGauge(float guage);

	bool IsRPossible();
	bool IsEPossible();

	void ApplyDash();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ChangeSkillIcon(int attacknumber);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void PlaySkillIconAnimation();

	FORCEINLINE float GetRecoveryDash() { return RecoveryRate; }
	FORCEINLINE void SetRecoveryDash(float NewValue) { RecoveryRate = NewValue; }

public :
	UFUNCTION(BlueprintCallable, Category = "Skill")
	bool SkillE();

	UFUNCTION(BlueprintCallable, Category = "Skill_UI")
	void HideUI();

	UFUNCTION(BlueprintCallable, Category = "Skill_UI")
	void ShowUI();

	UFUNCTION(BlueprintCallable, Category = "Skill_UI")
	void LockOnUI();

	UFUNCTION(BlueprintCallable, Category = "Skill_UI")
	void UpdatedESkillCool();
public :
	UFUNCTION(BlueprintCallable, Category = "Party_UI")
	void ChangeParty();
	
private :	
	void SetDash(float NewDash);
	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float MaxDash;
	UPROPERTY(Transient, VisibleAnywhere, Category = "Stat")
	float CurrentDash;
	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float RecoveryRate;
	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float coolTime_R;
	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float coolTime_E;
	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float RGauge = 0.0f;

	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float CurrentCoolE;

	bool bWasRPossible = false;
	bool bWasEPossible = false;
public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<TObjectPtr<class UPaperSprite>> NormalAttackIcons;

private:
	FTimerHandle RPossibleTimerHandle;
	FTimerHandle EPossibleTimerHandle;


	void PlayRAnimationLoop();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
	int32 partyNumber;
};
