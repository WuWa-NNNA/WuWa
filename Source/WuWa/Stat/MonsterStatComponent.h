// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stat/WWStatComponent.h"
#include "MonsterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPChangedDelegate, float /*CurrentHP*/);
/**
 * 
 */
UCLASS()
class WUWA_API UMonsterStatComponent : public UWWStatComponent
{
	GENERATED_BODY()
	
public:
    // 체력 변경 시 호출될 델리게이트
    FOnHPChangedDelegate OnHPChanged;

    // 체력을 깎는 함수 (예시)
    void SetHP(float NewHP);
};
