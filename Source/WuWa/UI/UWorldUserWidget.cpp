// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UWorldUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Stat/Player/PlayerStatComponent.h"
#include "PaperSprite.h"
#include "Stat/Monster/SigillumStatComponent.h"
#include "Misc/OutputDeviceNull.h"

UUWorldUserWidget::UUWorldUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UUWorldUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	LevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LevelText")));
	SkillImage = Cast<UImage>(GetWidgetFromName(TEXT("Base_Icon")));
	EHideImage = Cast<UImage>(GetWidgetFromName(TEXT("EHideImage")));
	ESkillKeyImage = Cast<UImage>(GetWidgetFromName(TEXT("W_ESkillKeyImage")));
	RHideImage = Cast<UImage>(GetWidgetFromName(TEXT("RHideImage")));
	BossName = Cast<UTextBlock>(GetWidgetFromName(TEXT("BossName")));
	BossHpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("BossHpBar")));
	BossParryBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("BossParryBar")));
	TransformationGauge = Cast<UProgressBar>(GetWidgetFromName(TEXT("TransformationGauge")));
	WBP_TOUCH1 = Cast<UUserWidget>(GetWidgetFromName(TEXT("W_Touch1")));
	WBP_TOUCH2 = Cast<UUserWidget>(GetWidgetFromName(TEXT("W_Touch2")));
	WBP_TOUCH3 = Cast<UUserWidget>(GetWidgetFromName(TEXT("W_Touch3")));
	WBP_TOUCH4 = Cast<UUserWidget>(GetWidgetFromName(TEXT("W_Touch4")));

	PartyIcon1 = Cast<UImage>(GetWidgetFromName(TEXT("W_PartyIcon1")));
	Party_WBP_TOUCH1 = Cast<UUserWidget>(GetWidgetFromName(TEXT("W_Party_WBP_TOUCH1")));
	PartyIconButton1 = Cast<UImage>(GetWidgetFromName(TEXT("W_PartyIconButton1")));
	PartyIcon2 = Cast<UImage>(GetWidgetFromName(TEXT("W_PartyIcon2")));
	Party_WBP_TOUCH2 = Cast<UUserWidget>(GetWidgetFromName(TEXT("W_Party_WBP_TOUCH2")));
	PartyIconButton2 = Cast<UImage>(GetWidgetFromName(TEXT("W_PartyIconButton2")));

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
	ensure(WBP_TOUCH1);
	ensure(WBP_TOUCH2);
	ensure(WBP_TOUCH3);
	ensure(WBP_TOUCH4);
	ensure(PartyIcon1);
	ensure(Party_WBP_TOUCH1);
	ensure(PartyIconButton1);
	ensure(PartyIcon2);
	ensure(Party_WBP_TOUCH2);
	ensure(PartyIconButton2);

	//if (DashBarImage)
	//{
	//	UMaterialInterface* BaseMaterial = DashBarImage->GetBrush().GetResourceObject() ? Cast<UMaterialInterface>(DashBarImage->GetBrush().GetResourceObject()) : nullptr;
	//	if (BaseMaterial)
	//	{
	//		DashGaugeMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	//		DashBarImage->SetBrushFromMaterial(DashGaugeMaterial);
	//	}
	//}

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
			//MaxDash = PlayerStatComp->GetMaxDash();
			UpdateHpBar(PlayerStatComp->GetCurrentHP());
			UpdateLevel(PlayerStatComp->GetLevel());
		}
	}

	SkillCoolEDisable();
	HideBossUI();
	
	WBP_TOUCH1->SetVisibility(ESlateVisibility::Hidden);
	WBP_TOUCH2->SetVisibility(ESlateVisibility::Hidden);
	WBP_TOUCH3->SetVisibility(ESlateVisibility::Hidden);
	WBP_TOUCH4->SetVisibility(ESlateVisibility::Hidden);
	Party_WBP_TOUCH1->SetVisibility(ESlateVisibility::Hidden);
	Party_WBP_TOUCH2->SetVisibility(ESlateVisibility::Hidden);

	UE_LOG(LogTemp,Log, TEXT("UMG NativeOnInitialized"));

	PartyIconButton1->SetVisibility(ESlateVisibility::Hidden);
	PartyIconButton2->SetVisibility(ESlateVisibility::Visible);
	TransformationGauge->SetVisibility(ESlateVisibility::Visible);
}

void UUWorldUserWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HPProgressBar)
	{
		HPProgressBar->SetPercent(NewCurrentHp / MaxHp);
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
		RGaugeMaterial->SetScalarParameterValue(TEXT("Progress"), 0);
		return;
	}
	else
	{
		RHideImage->SetVisibility(ESlateVisibility::Visible);
	}
	if (RGaugeMaterial)
	{
		RGaugeMaterial->SetScalarParameterValue(TEXT("Progress"), 1 - currenGauge);
	}
}

void UUWorldUserWidget::UpdateSkillIcon(UPaperSprite* NewIcon)
{
	ChangeIconSprite = NewIcon;
}

void UUWorldUserWidget::SkillCoolEActive(float a)
{
	EHideImage->SetVisibility(ESlateVisibility::Visible);
	ESkillKeyImage->SetVisibility(ESlateVisibility::Hidden);
	TriggerTouchAnimation(WBP_TOUCH3);
}

void UUWorldUserWidget::SkillCoolEDisable()
{
	EHideImage->SetVisibility(ESlateVisibility::Hidden);
	ESkillKeyImage->SetVisibility(ESlateVisibility::Visible);

}

void UUWorldUserWidget::UpdateAllVisuals(UPlayerStatComponent* Stat)
{
	if (!Stat) return;

	UpdateHpBar(Stat->GetCurrentHP());
}

void UUWorldUserWidget::InitializeBossUISetting(USigillumStatComponent* BossStat)
{

	BossHpBar->SetVisibility(ESlateVisibility::Visible);
	BossName->SetVisibility(ESlateVisibility::Visible);
	BossParryBar->SetVisibility(ESlateVisibility::Visible);


	BossMaxHp = BossStat->GetMaxHP();
	BossHpBar->SetPercent(BossStat->GetCurrentHP() / BossMaxHp);
	BossParryBar->SetPercent(BossStat->GetParryGauge());

	BossStat->OnHpChagned.RemoveAll(this);
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

void UUWorldUserWidget::HUDVisible()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UUWorldUserWidget::HUDHidden()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UUWorldUserWidget::TriggerTouchAnimation(UUserWidget* touchwidget)
{
	if (!touchwidget)
	{
		return;
	}
	if (touchwidget->GetVisibility() == ESlateVisibility::Hidden)
	{
		touchwidget->SetVisibility(ESlateVisibility::Visible);
	}
	FString FunctionName = TEXT("OnInteration");
	FOutputDeviceNull ar;
	touchwidget->CallFunctionByNameWithArguments(*FunctionName, ar, nullptr, true);
}

void UUWorldUserWidget::TriggerTouchLockOnAnimation()
{
	if (!WBP_TOUCH1)
	{
		return;
	}
	if (WBP_TOUCH1->GetVisibility() == ESlateVisibility::Hidden)
	{
		WBP_TOUCH1->SetVisibility(ESlateVisibility::Visible);
	}
	FString FunctionName = TEXT("OnInteration");
	FOutputDeviceNull ar;
	WBP_TOUCH1->CallFunctionByNameWithArguments(*FunctionName, ar, nullptr, true);
}

void UUWorldUserWidget::TriggerTouchBaseAttackAnimation()
{
	if (!WBP_TOUCH2)
	{
		return;
	}
	if (WBP_TOUCH2->GetVisibility() == ESlateVisibility::Hidden)
	{
		WBP_TOUCH2->SetVisibility(ESlateVisibility::Visible);
	}
	FString FunctionName = TEXT("OnInteration");
	FOutputDeviceNull ar;
	WBP_TOUCH2->CallFunctionByNameWithArguments(*FunctionName, ar, nullptr, true);
}

void UUWorldUserWidget::TriggerTouchBurstAnimation()
{
	if (!WBP_TOUCH4)
	{
		return;
	}
	if (WBP_TOUCH4->GetVisibility() == ESlateVisibility::Hidden)
	{
		WBP_TOUCH4->SetVisibility(ESlateVisibility::Visible);
	}
	FString FunctionName = TEXT("OnInteration");
	FOutputDeviceNull ar;
	WBP_TOUCH4->CallFunctionByNameWithArguments(*FunctionName, ar, nullptr, true);
}

void UUWorldUserWidget::UpdatePartyIcons(int32 partynumber)
{
	UE_LOG(LogTemp, Log, TEXT("party : %d"), partynumber);
	switch (partynumber)
	{
	case 1: // 유노
		TriggerTouchAnimation(Party_WBP_TOUCH2);
		PartyIconButton1->SetVisibility(ESlateVisibility::Hidden);		//1번 비활성화
		PartyIconButton2->SetVisibility(ESlateVisibility::Visible);		//2번 활성화
		TransformationGauge->SetVisibility(ESlateVisibility::Visible);
		break;
	case 2: // 치사
		TriggerTouchAnimation(Party_WBP_TOUCH1);
		PartyIconButton1->SetVisibility(ESlateVisibility::Visible);
		PartyIconButton2->SetVisibility(ESlateVisibility::Hidden);
		TransformationGauge->SetVisibility(ESlateVisibility::Hidden);
		break;
	default:
		break;
	}
}
