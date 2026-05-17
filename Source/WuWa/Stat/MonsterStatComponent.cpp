// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/MonsterStatComponent.h"

void UMonsterStatComponent::SetHP(float NewHP)
{
    CurrentHP = FMath::Clamp(NewHP, 0.0f, MaxHP);
    OnHPChanged.Broadcast(CurrentHP);
}
