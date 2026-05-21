// Fill out your copyright notice in the Description page of Project Settings.


#include "LunoStatComponent.h"
#include "Character/Resonator/Luno/Luno.h"


ULunoStatComponent::ULunoStatComponent()
{
    CrescentTime = 10.0f;
}



void ULunoStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(CrescentTimer);
    if (CrescentTime > RemainingTime)
    {
        OnChangeCrescentTime.Broadcast(RemainingTime/ CrescentTime);
    }
}

void ULunoStatComponent::ChangeAttackMode(ELunoState currentStatMode)
{
	FLunoIconGroup* FoundGroup = AttackIcon_Luno.Find(currentStatMode);

    if (FoundGroup)
    {
        NormalAttackIcons = FoundGroup->Icons;
    }
}

void ULunoStatComponent::SetCrescentTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(CrescentTimer);
    CrescentTimer.Invalidate();

    GetWorld()->GetTimerManager().SetTimer(CrescentTimer, this, &ULunoStatComponent::RevertLunoState, CrescentTime, false);
}

void ULunoStatComponent::RevertLunoState()
{
    Cast<ALuno>(GetOwner())->ChangeLunoState(ELunoState::Half);
    OnBaseEndCrescentTime.Broadcast();
}
