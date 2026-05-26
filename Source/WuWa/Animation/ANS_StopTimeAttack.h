// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_StopTimeAttack.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UANS_StopTimeAttack : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	// C++ 정석 가상 함수 이름인 NotifyBegin과 NotifyEnd를 사용합니다.
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};