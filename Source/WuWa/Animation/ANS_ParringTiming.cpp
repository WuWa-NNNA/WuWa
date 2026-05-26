// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS_ParringTiming.h"
#include "Interface/ParringTimingInterface.h"

void UANS_ParringTiming::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerActor = MeshComp->GetOwner();
	IParringTimingInterface* Monster = Cast<IParringTimingInterface>(OwnerActor);
	if (OwnerActor && Monster)
	{
		IParringTimingInterface::Execute_SetIsParringTiming(OwnerActor, true);
	}
}

void UANS_ParringTiming::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerActor = MeshComp->GetOwner();
	IParringTimingInterface* Monster = Cast<IParringTimingInterface>(OwnerActor);
	if (OwnerActor && Monster)
	{
		IParringTimingInterface::Execute_SetIsParringTiming(OwnerActor, false);
	}
}
