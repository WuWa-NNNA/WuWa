// Fill out your copyright notice in the Description page of Project Settings.


#include "LunoStatComponent.h"
#include "Character/Resonator/Luno/Luno.h"


ULunoStatComponent::ULunoStatComponent()
{
    CrescentTime = 15.0f;
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

void ULunoStatComponent::SetCrescentTimer()
{
	CrescentTimer.Invalidate();
    GetWorld()->GetTimerManager().SetTimer(CrescentTimer, this, &ULunoStatComponent::RevertLunoState, CrescentTime, false);
}

void ULunoStatComponent::RevertLunoState()
{
    Cast<ALuno>(GetOwner())->ChangeLunoState(ELunoState::Half);
}
