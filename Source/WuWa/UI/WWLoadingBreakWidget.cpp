// Fill out your copyright notice in the Description page of Project Settings.


#include "WWLoadingBreakWidget.h"
#include "Components/Image.h"

void UWWLoadingBreakWidget::NativeConstruct()
{
	LoadingImage = Cast<UImage>(GetWidgetFromName(TEXT("W_LoadingImage")));


	if (LoadingImage)
	{
		UMaterialInterface* BaseMaterial = LoadingImage->GetBrush().GetResourceObject() ? Cast<UMaterialInterface>(LoadingImage->GetBrush().GetResourceObject()) : nullptr;
		if (BaseMaterial)
		{
			LoadingImageaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
			LoadingImage->SetBrushFromMaterial(LoadingImageaterial);
		}
	}
}

void UWWLoadingBreakWidget::StartBreak()
{
	CurrentProgress = 0.f;

	if (LoadingImageaterial)
	{
		LoadingImageaterial->SetScalarParameterValue(TEXT("time"), 0.f);

		GetWorld()->GetTimerManager().SetTimer(
			BreakTimerHandle,
			this,
			&UWWLoadingBreakWidget::UpdateBreak,
			0.1f,
			true
		);
	}
}

void UWWLoadingBreakWidget::UpdateBreak()
{
	if (CurrentProgress < 0.3)
	{
		CurrentProgress += 0.05f;
	}
	else if(CurrentProgress > 0.7)
	{
		CurrentProgress += 0.06f;
	}
	else
	{
		CurrentProgress = 1.f;

		GetWorld()->GetTimerManager().ClearTimer(BreakTimerHandle);
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		RemoveFromParent();
	}

	LoadingImageaterial->SetScalarParameterValue(TEXT("time"), CurrentProgress);

	UE_LOG(LogTemp, Log, TEXT("LoadingImageaterial : %f"), CurrentProgress);

}