// Fill out your copyright notice in the Description page of Project Settings.


#include "LunoStatComponent.h"



ULunoStatComponent::ULunoStatComponent()
{
}

void ULunoStatComponent::AttackChange()
{
	UE_LOG(LogTemp, Log, TEXT("SkillR"));

	OnBaseAttack.Broadcast();
}

void ULunoStatComponent::ChangeAttackMode(ELunoState currentStatMode)
{
	FLunoIconGroup* FoundGroup = AttackIcon_Luno.Find(currentStatMode);

    if (FoundGroup)
    {
        NormalAttackIcons = FoundGroup->Icons;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ChangeAttackMode: Could not find icons for the current mode!"));
    }
}
