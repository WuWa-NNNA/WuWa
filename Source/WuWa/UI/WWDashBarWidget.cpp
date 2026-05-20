// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WWDashBarWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UWWDashBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DashGaugeImage = Cast<UImage>(GetWidgetFromName(TEXT("UDashGaugeImage")));
	ensure(DashGaugeImage);

	if (DashGaugeImage)
	{
		UMaterialInterface* BaseMaterial = DashGaugeImage->GetBrush().GetResourceObject() ? Cast<UMaterialInterface>(DashGaugeImage->GetBrush().GetResourceObject()) : nullptr;
		if (BaseMaterial)
		{
			DashGaugeMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
			DashGaugeImage->SetBrushFromMaterial(DashGaugeMaterial);
		}
	}
	MaxDash = 10;
}


void UWWDashBarWidget::UpdataWorldDash(float NewCurrentDash)
{
	//UE_LOG(LogTemp, Log, TEXT("UpdataWorldDash1"));
	if (DashGaugeMaterial)
	{
		float DashPercent = NewCurrentDash / MaxDash;

		DashGaugeMaterial->SetScalarParameterValue(TEXT("Progress"), 1 - DashPercent);
		//UE_LOG(LogTemp, Log, TEXT("UpdataWorldDash2"));

		if (DashPercent >= 1)
		{
			DashGaugeImage->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			DashGaugeImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (DashPercent < 0.4)
		{
			DashGaugeMaterial->SetVectorParameterValue(TEXT("Ba_Color"), FVector3d(0.5f, 0.0f, 0.f));
		}
		else //(R=0.381326,G=0.327778,B=0.102242,A=1.000000)
		{
			DashGaugeMaterial->SetVectorParameterValue(TEXT("Ba_Color"), FVector3d(0.381326f, 0.327778f, 0.102242f));
		}

	}
}
