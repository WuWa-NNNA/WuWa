// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UWorldUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

#include "Interface/WWCharacterWidgetInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Stat/Player/PlayerStatComponent.h"
#include "PaperSprite.h"
#include "WWMonsterWidget.h"
#include "Stat/Monster/SigillumStatComponent.h"

UUWorldUserWidget::UUWorldUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
	MaxDash = -1.0f;
}

void UUWorldUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	LevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LevelText")));
	SkillImage = Cast<UImage>(GetWidgetFromName(TEXT("Base_Icon")));
	EHideImage = Cast<UImage>(GetWidgetFromName(TEXT("EHideImage")));
	ESkillKeyImage = Cast<UImage>(GetWidgetFromName(TEXT("ESkillKeyImage")));
	RHideImage = Cast<UImage>(GetWidgetFromName(TEXT("RHideImage")));
	BossName = Cast<UTextBlock>(GetWidgetFromName(TEXT("BossName")));
	BossHpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("BossHpBar")));
	BossParryBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("BossParryBar")));
	TransformationGauge = Cast<UProgressBar>(GetWidgetFromName(TEXT("TransformationGauge")));
	
	ensure(HPProgressBar);
	ensure(LevelText);
	ensure(SkillImage);
	ensure(EHideImage);
	ensure(RHideImage);
	ensure(ESkillKeyImage);
	ensure(BossName);
	ensure(BossHpBar);
	ensure(BossParryBar);
	ensure(TransformationGauge);

	if (DashBarImage)
	{
		UMaterialInterface* BaseMaterial = DashBarImage->GetBrush().GetResourceObject() ? Cast<UMaterialInterface>(DashBarImage->GetBrush().GetResourceObject()) : nullptr;
		if (BaseMaterial)
		{
			// 다이나믹 인스턴스 생성
			DashGaugeMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
			// 생성된 다이나믹 머터리얼을 다시 이미지의 브러시에 할당
			DashBarImage->SetBrushFromMaterial(DashGaugeMaterial);
		}
	}
	if (RBarImage)
	{
		UMaterialInterface* BaseMaterial = RBarImage->GetBrush().GetResourceObject() ? Cast<UMaterialInterface>(RBarImage->GetBrush().GetResourceObject()) : nullptr;
		if (BaseMaterial)
		{
			RGaugeMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
			RBarImage->SetBrushFromMaterial(RGaugeMaterial);
		}
	}
	AActor* TargetActor = OwningActor ? OwningActor : GetOwningPlayerPawn();
	if (TargetActor)
	{
		UPlayerStatComponent* PlayerStatComp = TargetActor->FindComponentByClass<UPlayerStatComponent>();
		if (PlayerStatComp)
		{
			MaxHp = PlayerStatComp->GetMaxHP();
			MaxDash = PlayerStatComp->GetMaxDash();
			UpdateHpBar(PlayerStatComp->GetCurrentHP());
			UpdateMainHUD(PlayerStatComp->GetCurrentDash());
			UpdateLevel(PlayerStatComp->GetLevel());
		}
	}
		IWWCharacterWidgetInterface* CharacterWidget = Cast<IWWCharacterWidgetInterface>(TargetActor);
		if (CharacterWidget)
		{
			CharacterWidget->SetupCharacterWidget(this);
		}

	SkillCoolEDisable();
	HideBossUI();

}

void UUWorldUserWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HPProgressBar)
	{
		HPProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}

}

void UUWorldUserWidget::UpdateMainHUD(float NewCurrentDash)
{
	ensure(MaxDash > 0.0f);

	if (DashGaugeMaterial)
	{
		float DashPercent = NewCurrentDash / MaxDash;

		DashGaugeMaterial->SetScalarParameterValue(TEXT("Progress"), 1 - DashPercent);
		if (DashPercent >= 1)
		{
			DashBarImage->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			DashBarImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (DashPercent < 0.4)
		{
			DashGaugeMaterial->SetVectorParameterValue(TEXT("Ba_Color"),FVector3d(0.5f,0.0f, 0.f));
		}
		else //(R=0.381326,G=0.327778,B=0.102242,A=1.000000)
		{
			DashGaugeMaterial->SetVectorParameterValue(TEXT("Ba_Color"), FVector3d(0.381326f, 0.327778f, 0.102242f));
		}

	}
}



void UUWorldUserWidget::UpdateLevel(int Level)
{
	LevelText->SetText(FText::Format(FText::FromString("LV. {0}"), FText::AsNumber(Level)));
}

void UUWorldUserWidget::UpdateRGauge(float currenGauge)
{
	
	if (currenGauge >= 1)
	{
		RHideImage->SetVisibility(ESlateVisibility::Hidden);
		RGaugeMaterial->SetScalarParameterValue(TEXT("Progress"), 1);
		return;
	}
	else
	{
		RHideImage->SetVisibility(ESlateVisibility::Visible);
	}
	if (RGaugeMaterial)
	{
		RGaugeMaterial->SetScalarParameterValue(TEXT("Progress"), currenGauge);
		//UE_LOG(LogTemp, Log, TEXT("R ++"));
	}
	
}

void UUWorldUserWidget::UpdateSkillIcon(UPaperSprite* NewIcon)
{
	ChangeIconSprite = NewIcon;
}

void UUWorldUserWidget::SkillCoolEActive(float a)
{
	//UE_LOG(LogTemp, Log, TEXT("SkillE - Start"));
	EHideImage->SetVisibility(ESlateVisibility::Visible);
	ESkillKeyImage->SetVisibility(ESlateVisibility::Hidden);
}

void UUWorldUserWidget::SkillCoolEDisable()
{
	//UE_LOG(LogTemp, Log, TEXT("SkillE - End"));
	EHideImage->SetVisibility(ESlateVisibility::Hidden);
	ESkillKeyImage->SetVisibility(ESlateVisibility::Visible);

}



void UUWorldUserWidget::UpdateSkillCoolR(float coolTime)
{
	UE_LOG(LogTemp, Log, TEXT("UpdateSkillCoolR"));

}

void UUWorldUserWidget::UpdateAllVisuals(UPlayerStatComponent* Stat)
{
	if (!Stat) return;

	UpdateHpBar(Stat->GetCurrentHP());
	UpdateMainHUD(Stat->GetCurrentDash());
}

void UUWorldUserWidget::InitializeBossUISetting(USigillumStatComponent* BossStat)
{

	BossHpBar->SetVisibility(ESlateVisibility::Visible);
	BossName->SetVisibility(ESlateVisibility::Visible);
	BossParryBar->SetVisibility(ESlateVisibility::Visible);


	BossMaxHp = BossStat->GetMaxHP();
	BossHpBar->SetPercent(BossStat->GetCurrentHP() / BossMaxHp);
	BossParryBar->SetPercent(BossStat->GetParryGauge());

	BossStat->OnHpChagned.Clear();
	BossStat->OnHpChagned.AddUObject(this, &UUWorldUserWidget::Damaged);
	UE_LOG(LogTemp, Log, TEXT("Succed InitializeBossUISetting"));
}


void UUWorldUserWidget::HideBossUI()
{
	BossHpBar->SetVisibility(ESlateVisibility::Collapsed);
	BossName->SetVisibility(ESlateVisibility::Collapsed);
	BossParryBar->SetVisibility(ESlateVisibility::Collapsed);
}

void UUWorldUserWidget::UpdateBossHpBar(float currentHp, float currentParry)
{
	BossHpBar->SetPercent(currentHp/BossMaxHp);
	BossParryBar->SetPercent(currentParry);
}

void UUWorldUserWidget::Damaged(float damaged)
{
	BossHpBar->SetPercent(damaged / BossMaxHp);
}

void UUWorldUserWidget::ChangedTransformationGauge(float Gauge)
{
	TransformationGauge->SetPercent(Gauge);
}
