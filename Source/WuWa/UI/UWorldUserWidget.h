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
	virtual void NativeConstruct() override;

public :
	FORCEINLINE void SetMaxHp(float NewMaxHP) { MaxHp = NewMaxHP; }

	UFUNCTION()
	void UpdateHpBar(float NewCurrentHp);

	FORCEINLINE void SetMaxDash(float NewMaxDash) { MaxDash = NewMaxDash; }
	UFUNCTION()
	void UpdateMainHUD(float NewCurrentDash);

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
	void UpdateSkillCoolR(float coolTime);
	
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
protected :
	UPROPERTY()
	TObjectPtr<class UProgressBar> HPProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> DashBarImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> RBarImage;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> DashGaugeMaterial;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> RGaugeMaterial;
	
	UPROPERTY()
	TObjectPtr<class UTextBlock> LevelText;

	UPROPERTY()
	TObjectPtr<class UImage> SkillImage;

	UPROPERTY()
	TObjectPtr<class UImage> ESkillKeyImage;

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float MaxDash;

	UPROPERTY()
	float BossMaxHp;

	UPROPERTY()
	TObjectPtr<class UImage> EHideImage;

	UPROPERTY()
	TObjectPtr<class UImage> RHideImage;

	UPROPERTY()
	TObjectPtr<class UProgressBar> TransformationGauge;

protected : // 보스
	UPROPERTY()
	TObjectPtr<class UTextBlock> BossName;

	UPROPERTY()
	TObjectPtr<class UProgressBar> BossHpBar;

	UPROPERTY()
	TObjectPtr<class UProgressBar> BossParryBar;

public :
	void UpdateAllVisuals(class UPlayerStatComponent* Stat);

	void InitializeBossUISetting(class USigillumStatComponent* BossStat);

public :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UPaperSprite> ChangeIconSprite;

};
