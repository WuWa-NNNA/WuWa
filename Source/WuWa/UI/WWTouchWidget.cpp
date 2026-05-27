// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WWTouchWidget.h"
#include "Components/Image.h"

void UWWTouchWidget::ChangeImageColor(FLinearColor color)
{
	Image1->SetColorAndOpacity(color);
	Image2->SetColorAndOpacity(color);
	Image3->SetColorAndOpacity(color);
	Image4->SetColorAndOpacity(color);
}
