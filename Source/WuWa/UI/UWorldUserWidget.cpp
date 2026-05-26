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

#include "Data/WWSkillIconData.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
UUWorldUserWidget::UUWorldUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObj(TEXT("/Game/PCH/Data/SkillIconData.SkillIconData"));

	if (DataTableObj.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("DataTableObj Succese"));

		SkillDataTable = DataTableObj.Object;
	}
	CurrentPartyNumber = 0;
}

void UUWorldUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	LevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LevelText")));
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

	//UE_LOG(LogTemp,Log, TEXT("UMG NativeOnInitialized"));

	PartyIconButton1->SetVisibility(ESlateVisibility::Hidden);
	PartyIconButton2->SetVisibility(ESlateVisibility::Visible);
	TransformationGauge->SetVisibility(ESlateVisibility::Visible);
	
	PartyHpBar1->SetPercent(1);
	PartyHpBar2->SetPercent(1);
}

void UUWorldUserWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HPProgressBar)
	{
		HPProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
	if (CurrentPartyNumber == 1)
	{
		PartyHpBar1->SetPercent(NewCurrentHp / MaxHp);
	}
	if (CurrentPartyNumber == 2)
	{
		PartyHpBar2->SetPercent(NewCurrentHp / MaxHp);
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
	//ChangeIconSprite = NewIcon;
	BaseSkillIcon->SetBrushFromAtlasInterface(NewIcon);
	//UE_LOG(LogTemp, Log, TEXT("UpdateSkillIcon %s : "), *NewIcon->GetName());
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
	BossStat->OnParry.AddUObject(this, &UUWorldUserWidget::UpdateBossParyGauge);
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
		TriggerTouchAnimation(Party_WBP_TOUCH1);
		PartyIconButton1->SetVisibility(ESlateVisibility::Hidden);		//1번 비활성화
		PartyIconButton2->SetVisibility(ESlateVisibility::Visible);		//2번 활성화
		TransformationGauge->SetVisibility(ESlateVisibility::Visible);
		Party_WBP_TOUCH2->SetVisibility(ESlateVisibility::Hidden);
		break;
	case 2: // 치사
		TriggerTouchAnimation(Party_WBP_TOUCH2);
		PartyIconButton1->SetVisibility(ESlateVisibility::Visible);
		PartyIconButton2->SetVisibility(ESlateVisibility::Hidden);
		TransformationGauge->SetVisibility(ESlateVisibility::Hidden);
		Party_WBP_TOUCH1->SetVisibility(ESlateVisibility::Hidden);
		break;
	default:
		break;
	}
	UpdatePartySkillIcons(partynumber);
}

void UUWorldUserWidget::UpdatePartySkillIcons(int32 partynumber)
{
	if(!SkillDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillDataTable is null"));
		return;
	}

	FName RowNameE;
	FName RowNameR;
	FName RowNameBase;
	CurrentPartyNumber = partynumber;
	if (partynumber == 1)
	{
		RowNameE = TEXT("Luno_E");
		RowNameR = TEXT("Luno_R");
		RowNameBase = TEXT("Luno_Normal_1");

	}
	else if (partynumber == 2)
	{
		RowNameE = TEXT("Chisa_E");
		RowNameR = TEXT("Chisa_R");
		RowNameBase = TEXT("Chisa_Normal_1");
	}

	FWWSkillIconData* SkillDataE = SkillDataTable->FindRow<FWWSkillIconData>(RowNameE, TEXT("UpdatePartySkillIcons"));
	FWWSkillIconData* SkillDataR = SkillDataTable->FindRow<FWWSkillIconData>(RowNameR, TEXT("UpdatePartySkillIcons"));
	FWWSkillIconData* SkillDataBase = SkillDataTable->FindRow<FWWSkillIconData>(RowNameBase, TEXT("UpdatePartySkillIcons"));


	UPaperSprite* SpriteE = SkillDataE->SkillIcon.LoadSynchronous();
	UPaperSprite* SpriteR = SkillDataR->SkillIcon.LoadSynchronous();
	UPaperSprite* SpriteBase = SkillDataBase->SkillIcon.LoadSynchronous();

	if (!SpriteE && ! SpriteR && !SpriteBase)
	{
		UE_LOG(LogTemp, Warning,TEXT("Sprite Load Failed"));
		return;
	}
	ESkillIconImage->SetBrushFromAtlasInterface(SpriteE);
	RSkillIconImage->SetBrushFromAtlasInterface(SpriteR);
	BaseSkillIcon->SetBrushFromAtlasInterface(SpriteBase);

}

void UUWorldUserWidget::UpdateBossParyGauge(float gauge)
{
	UE_LOG(LogTemp, Log, TEXT("parry %f"), gauge);
	BossParryBar->SetPercent(gauge);
}
