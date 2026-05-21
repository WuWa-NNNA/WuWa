// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WWWidgetComponent.h"
#include "UI/WWUserWidget.h"



void UWWWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UWWUserWidget* WWUserWidget = Cast<UWWUserWidget>(GetWidget());
	if (WWUserWidget)
	{
		WWUserWidget->SetOwningActor(GetOwner());
	}

}
