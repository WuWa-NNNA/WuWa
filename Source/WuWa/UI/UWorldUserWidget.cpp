// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UWorldUserWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/WWCharacterWidgetInterface.h"

UUWorldUserWidget::UUWorldUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
	MaxDash = 10.f;
}

void UUWorldUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetMaxDash(10);
	SetMaxHp(10);
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPWidget")));
	ensure(HPProgressBar);
	// 그냥 블루프린트 할당되게 변경
	IWWCharacterWidgetInterface* characterWIdget = Cast<IWWCharacterWidgetInterface>(OwningActor);
	if (characterWIdget)
	{
		characterWIdget->SetupCharacterWidget(this);
	}
	
}

void UUWorldUserWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HPProgressBar)
	{
		HPProgressBar->SetPercent(NewCurrentHp / MaxHp);
		UE_LOG(LogTemp, Log, TEXT("down HP"));

	}

	//OwningActor
}

void UUWorldUserWidget::UpdateDashBar(float NewCurrentDash)
{
	ensure(MaxHp > 0.0f);
	if (HPProgressBar)
	{
		
		HPProgressBar->SetPercent(NewCurrentDash / MaxDash);
		UE_LOG(LogTemp, Log, TEXT("down HP"));

	}

	ensure(MaxDash > 0.0f);
	if (DashProgressBar)
	{
		DashProgressBar->SetPercent(NewCurrentDash / MaxDash);
		UE_LOG(LogTemp, Log, TEXT("down Dash"));
	}
}
