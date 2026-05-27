// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/Player/PlayerStatComponent.h"
#include "PaperSprite.h"

UPlayerStatComponent::UPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxDash = 10.f;
	CurrentDash = MaxDash;

	RecoveryRate = 0.8f;

	coolTime_E = 10.0f;
	coolTime_R = 5.0f;
	partyNumber = 0;
	CurrentCoolE = 0;
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetDash(GetMaxDash()-0.1f);
}

void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentDash < MaxDash)
	{
		CurrentDash = FMath::Clamp(CurrentDash + (RecoveryRate * DeltaTime), 0.0f, MaxDash);
		OnDashChanged.Broadcast(CurrentDash);
	}

	if (IsRPossible())
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(RPossibleTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				RPossibleTimerHandle,
				this,
				&UPlayerStatComponent::PlayRAnimationLoop,
				0.2f,
				true
			);
			OnPlayIcon4Animation.Broadcast();
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(RPossibleTimerHandle);
	}
}

void UPlayerStatComponent::PlayRAnimationLoop()
{
	if (!IsRPossible())
	{
		GetWorld()->GetTimerManager().ClearTimer(RPossibleTimerHandle);
		return;
	}

	OnPlayIcon4Animation.Broadcast();
}

void UPlayerStatComponent::SetRGauge(float guage)
{
	RGauge = guage;
	OnRGaugaChanged.Broadcast(guage);
}

bool UPlayerStatComponent::IsRPossible()
{
	return (RGauge >= 1.0f);
}

bool UPlayerStatComponent::IsEPossible()
{
	return (CurrentCoolE <= 0.0f);
}

void UPlayerStatComponent::ApplyDash()
{
	const float PrevHP = CurrentDash;
	if (CurrentDash < 0.75f)
	{
		return;
	}
	SetDash(CurrentDash - 0.75f);
}

void UPlayerStatComponent::ChangeSkillIcon(int attacknumber)
{
	uint32 tempattacknumber = 0;

	if (attacknumber >= 3)
	{
		tempattacknumber = 0;
	}
	else
	{
		tempattacknumber = attacknumber;
	}

	if (NormalAttackIcons.IsValidIndex(tempattacknumber))
	{
		UPaperSprite* SelectedIcon = NormalAttackIcons[tempattacknumber];
		if (SelectedIcon)
		{
			OnBaseSkillchange.Broadcast(SelectedIcon);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("no sprite : %d"), tempattacknumber);
	}
}

void UPlayerStatComponent::PlaySkillIconAnimation()
{
	OnPlayIconAnimation.Broadcast();
}

bool UPlayerStatComponent::SkillE()
{
	if (!IsEPossible())
	{
		return false;
	}
	CurrentCoolE = coolTime_E;
	UE_LOG(LogTemp, Log, TEXT("SkillE"));
	OnSkillEStart2.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(
		EPossibleTimerHandle,
		this,
		&UPlayerStatComponent::UpdatedESkillCool,
		0.1f,
		true
	);
	return true;
}

void UPlayerStatComponent::SetDash(float NewDash)
{
	CurrentDash = FMath::Clamp<float>(NewDash, 0.0f, MaxDash);
	OnDashChanged.Broadcast(CurrentDash);
}

void UPlayerStatComponent::HideUI()
{
	OnRSart.Broadcast();
}

void UPlayerStatComponent::ShowUI()
{
	OnREnd.Broadcast();
}

void UPlayerStatComponent::LockOnUI()
{
	OnLockOn.Broadcast();
}

void UPlayerStatComponent::UpdatedESkillCool()
{
	CurrentCoolE = FMath::Clamp(CurrentCoolE - 0.1f, 0.0f, coolTime_E);
	FOnSkillEStart.Broadcast(CurrentCoolE);
}

void UPlayerStatComponent::ChangeParty()
{
	OnPartyChanged.Broadcast(partyNumber);
	OnDashChanged.Broadcast(CurrentDash);
}
