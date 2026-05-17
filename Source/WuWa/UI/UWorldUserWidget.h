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
	void UpdateSkillIcon(class UPaperSprite* NewIcon);

	UFUNCTION()
	void SkillCoolEActive(float a);

	UFUNCTION()
	void SkillCoolEDisable();
	
	UFUNCTION()
	void UpdateSkillCoolR(float coolTime);

protected :
	UPROPERTY()
	TObjectPtr<class UProgressBar> HPProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> DashBarImage;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> ProgressDynamicMaterial;
	
	UPROPERTY()
	TObjectPtr<class UTextBlock>  LevelText;

	UPROPERTY()
	TObjectPtr<class UImage>  SkillImage;

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float MaxDash;

	UPROPERTY()
	TObjectPtr<class UImage> Skill_E_Active_Image;

public :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UPaperSprite> ChangeIconSprite;

};
