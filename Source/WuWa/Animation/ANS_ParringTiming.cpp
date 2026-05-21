// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS_ParringTiming.h"
#include "Interface/ParringTimingInterface.h"

void UANS_ParringTiming::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	IParringTimingInterface* Monster = Cast<IParringTimingInterface>(MeshComp->GetOwner());
	if (Monster)
	{
		Monster->SetIsParringTiming(true);
	}
}

void UANS_ParringTiming::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	IParringTimingInterface* Monster = Cast<IParringTimingInterface>(MeshComp->GetOwner());
	if (Monster)
	{
		Monster->SetIsParringTiming(false);
	}
}
