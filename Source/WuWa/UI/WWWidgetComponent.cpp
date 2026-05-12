// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WWWidgetComponent.h"
#include "UI/WWUserWidget.h"



void UWWWidgetComponent::InitWidget()
{
	Super::InitWidget();

	// InitWiget() 상위 로직을 따라가보면 함수 실행과정에서

	UWWUserWidget* WWUserWidget = Cast<UWWUserWidget>(GetWidget());
	if (WWUserWidget)
	{
		WWUserWidget->SetOwningActor(GetOwner());
	}

}
