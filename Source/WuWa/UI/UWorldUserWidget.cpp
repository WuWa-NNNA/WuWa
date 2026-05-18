// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UWorldUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

#include "Interface/WWCharacterWidgetInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Stat/PlayerStatComponent.h"
#include "PaperSprite.h"

UUWorldUserWidget::UUWorldUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
	MaxDash = -1.0f;
}

void UUWorldUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AActor* TargetActor = OwningActor;
	if (TargetActor == nullptr)
	{
		TargetActor = GetOwningPlayerPawn();
	}
	if (TargetActor)
	{
		UPlayerStatComponent* PlayerStatComp = TargetActor->FindComponentByClass<UPlayerStatComponent>();
		if (PlayerStatComp)
		{
			MaxHp = PlayerStatComp->GetMaxHP();
			MaxDash = PlayerStatComp->GetMaxDash();
			SetMaxDash(MaxDash);
			SetMaxHp(MaxHp);
		}
		IWWCharacterWidgetInterface* CharacterWidget = Cast<IWWCharacterWidgetInterface>(TargetActor);
		if (CharacterWidget)
		{
			CharacterWidget->SetupCharacterWidget(this);
		}
	}

	
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	ensure(HPProgressBar);

	LevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LevelText")));
	ensure(LevelText);

	SkillImage = Cast<UImage>(GetWidgetFromName(TEXT("Base_Icon")));
	ensure(SkillImage);

	Skill_E_Active_Image = Cast<UImage>(GetWidgetFromName(TEXT("Skill_E_Active_Image")));
	ensure(Skill_E_Active_Image);

	UMaterialInterface* BaseMaterial = DashBarImage->GetBrush().GetResourceObject() ? Cast<UMaterialInterface>(DashBarImage->GetBrush().GetResourceObject()) : nullptr;

	if (BaseMaterial)
	{
		// 다이나믹 인스턴스 생성
		ProgressDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		// 생성된 다이나믹 머터리얼을 다시 이미지의 브러시에 할당
		DashBarImage->SetBrushFromMaterial(ProgressDynamicMaterial);
	}

	SkillCoolEDisable();
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

	if (ProgressDynamicMaterial)
	{
		float DashPercent = NewCurrentDash / MaxDash;

		ProgressDynamicMaterial->SetScalarParameterValue(TEXT("Progress"), 1 - DashPercent);

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
			ProgressDynamicMaterial->SetVectorParameterValue(TEXT("Ba_Color"),FVector3d(0.5f,0.0f, 0.f));
		}
		else //(R=0.381326,G=0.327778,B=0.102242,A=1.000000)
		{
			ProgressDynamicMaterial->SetVectorParameterValue(TEXT("Ba_Color"), FVector3d(0.381326f, 0.327778f, 0.102242f));
		}

	}
}



void UUWorldUserWidget::UpdateLevel(int Level)
{
	UPlayerStatComponent* PlayerStatComp = OwningActor->FindComponentByClass<UPlayerStatComponent>();
	if (PlayerStatComp)
	if (PlayerStatComp)
	{
		LevelText->SetText(FText::Format(FText::FromString("LV. {0}"), FText::AsNumber(Level)));
	}
}

void UUWorldUserWidget::UpdateSkillIcon(UPaperSprite* NewIcon)
{
	ChangeIconSprite = NewIcon;
}

void UUWorldUserWidget::SkillCoolEActive(float a)
{
	UE_LOG(LogTemp, Log, TEXT("SkillE - Start"));
	Skill_E_Active_Image->SetVisibility(ESlateVisibility::Visible);
}

void UUWorldUserWidget::SkillCoolEDisable()
{
	UE_LOG(LogTemp, Log, TEXT("SkillE - End"));
	Skill_E_Active_Image->SetVisibility(ESlateVisibility::Hidden);
}



void UUWorldUserWidget::UpdateSkillCoolR(float coolTime)
{
	UE_LOG(LogTemp, Log, TEXT("UpdateSkillCoolR"));

}
