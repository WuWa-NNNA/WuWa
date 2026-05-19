// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Stat/WWStatComponent.h"
#include "PlayerStatComponent.generated.h"



DECLARE_MULTICAST_DELEGATE_OneParam(FOnDashChangedDelegate, float /*currentDash*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillEStartDelegate, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillRStartDelegate, float);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBaseSkillchangeDelegate,class UPaperSprite* /*skillIcon*/);




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
	FOnDashChangedDelegate OnDashChanged;
	FOnSkillEStartDelegate FOnSkillEStart;
	FOnSkillRStartDelegate FOnSkillRStart;

	FOnBaseSkillchangeDelegate OnBaseSkillchange;

	FORCEINLINE float GetMaxDash() { return MaxDash; }
	FORCEINLINE float GetCurrentDash() { return CurrentDash; }
	FORCEINLINE void SetCurrentDash(float change) { CurrentDash = change; }

	FORCEINLINE float GetCoolTime_E() { return coolTime_E; }
	FORCEINLINE void SetCoolTime_E(float change) { coolTime_E = change; }


	void ApplyDash();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ChangeSkillIcon(int attacknumber);

	FORCEINLINE float GetRecoveryDash() { return RecoveryRate; }

public :
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SkillE();
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SkillR();
	
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


public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<TObjectPtr<class UPaperSprite>> NormalAttackIcons;

};
