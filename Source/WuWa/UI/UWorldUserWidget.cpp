// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UWorldUserWidget.h"
#include "Components/ProgressBar.h"

UUWorldUserWidget::UUWorldUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
	MaxDash = -1.0f;
}

void UUWorldUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBarProgress")));
	//ensure(HPProgressBar);
	// Todo : 그냥 블루프린트 할당되게 변경

}

void UUWorldUserWidget::UpdateHpBar(float NewCurrentHp)
{
	/*ensure(MaxHp > 0.0f);
	if (HPProgressBar)
	{
		HPProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}*/
}

//void UUWorldUserWidget::UpdateDashBar(float NewCurrentDash)
//{
//	ensure(MaxDash > 0.0f);
//	if (DashProgressBar)
//	{
//		DashProgressBar->SetPercent(NewCurrentDash / MaxDash);
//	}
//}
