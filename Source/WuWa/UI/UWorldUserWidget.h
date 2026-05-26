// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WWUserWidget.h"
#include "UWorldUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UUWorldUserWidget : public UWWUserWidget
{
	GENERATED_BODY()
	
public :
	UUWorldUserWidget(const FObjectInitializer& ObjectInitializer);
protected :
	virtual void NativeOnInitialized() override;

public :
	FORCEINLINE void SetMaxHp(float NewMaxHP) { MaxHp = NewMaxHP; }

	UFUNCTION()
	void UpdateHpBar(float NewCurrentHp);

	UFUNCTION()
	void UpdateLevel(int Level);

	UFUNCTION()
	void UpdateRGauge(float currenGauge);

	UFUNCTION()
	void UpdateSkillIcon(class UPaperSprite* NewIcon);

	UFUNCTION()
	void SkillCoolEActive(float a);

	UFUNCTION()
	void SkillCoolEDisable();
	
	UFUNCTION()
	void HideBossUI();

	UFUNCTION()
	void UpdateBossHpBar(float currentHp, float currentParry);

	UFUNCTION()
	void Damaged(float damaged);
	
	UFUNCTION()
	void ChangedTransformationGauge(float Gauge);

public :
	UFUNCTION()
	void HUDVisible();

	UFUNCTION()
	void HUDHidden();

	UFUNCTION()
	void TriggerTouchAnimation(UUserWidget* touchwidget);

	UFUNCTION()
	void TriggerTouchLockOnAnimation();

	UFUNCTION()
	void TriggerTouchBaseAttackAnimation();

	UFUNCTION()
	void TriggerTouchBurstAnimation();

	UFUNCTION()
	void UpdatePartyIcons(int32 partynumber);

	UFUNCTION()
	void UpdatePartySkillIcons(int32 partynumber);

public :
	UFUNCTION()
	void UpdateBossParyGauge(float gauge);

protected :
	UPROPERTY()
	TObjectPtr<class UProgressBar> HPProgressBar;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UImage> DashBarImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> RBarImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> RSkillIconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ESkillIconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> BaseSkillIcon;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> DashGaugeMaterial;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> RGaugeMaterial;
	
	UPROPERTY()
	TObjectPtr<class UTextBlock> LevelText;



	UPROPERTY()
	TObjectPtr<class UImage> ESkillKeyImage;

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float BossMaxHp;

	UPROPERTY()
	TObjectPtr<class UImage> EHideImage;

	UPROPERTY()
	TObjectPtr<class UImage> RHideImage;

	UPROPERTY()
	TObjectPtr<class UProgressBar> TransformationGauge;

	UPROPERTY()
	TObjectPtr<UUserWidget> WBP_TOUCH1;

	UPROPERTY()
	TObjectPtr<UUserWidget> WBP_TOUCH2;

	UPROPERTY()
	TObjectPtr<UUserWidget> WBP_TOUCH3;

	UPROPERTY()
	TObjectPtr<UUserWidget> WBP_TOUCH4;

protected : // 보스
	UPROPERTY()
	TObjectPtr<class UTextBlock> BossName;

	UPROPERTY()
	TObjectPtr<class UProgressBar> BossHpBar;

	UPROPERTY()
	TObjectPtr<class UProgressBar> BossParryBar;

protected :	// 파티원 아이콘
	UPROPERTY()
	TObjectPtr<class UImage> PartyIcon1;
	UPROPERTY()
	TObjectPtr<class UUserWidget> Party_WBP_TOUCH1;
	UPROPERTY()
	TObjectPtr<class UImage> PartyIconButton1;

	UPROPERTY()
	TObjectPtr<class UImage> PartyIcon2;
	UPROPERTY()
	TObjectPtr<class UUserWidget> Party_WBP_TOUCH2;
	UPROPERTY()
	TObjectPtr<class UImage> PartyIconButton2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PartyHpBar1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PartyHpBar2;

	int32 CurrentPartyNumber;

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> SkillDataTable;
public :
	void UpdateAllVisuals(class UPlayerStatComponent* Stat);

	void InitializeBossUISetting(class USigillumStatComponent* BossStat);

public :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UPaperSprite> ChangeIconSprite;
};
