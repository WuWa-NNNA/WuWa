// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/Player/PlayerStatComponent.h"
#include "PaperSprite.h"



// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxDash = 10.f;
	CurrentDash = MaxDash;

	RecoveryRate = 0.2f;

	coolTime_E = 1.0f;
	coolTime_R = 5.0f;
	/*NormalAttackIcons.SetNum(3);

	static ConstructorHelpers::FObjectFinder<UPaperSprite> Icon1(TEXT("/Game/PCH/Asset/UIImage/YounuoSkill/LB1.LB1"));
	static ConstructorHelpers::FObjectFinder<UPaperSprite> Icon2(TEXT("/Game/PCH/Asset/UIImage/YounuoSkill/LB2.LB2"));
	static ConstructorHelpers::FObjectFinder<UPaperSprite> Icon3(TEXT("/Game/PCH/Asset/UIImage/YounuoSkill/LB3.LB3"));


	if (Icon1.Succeeded())
	{
		NormalAttackIcons[0] = Icon1.Object;
		UE_LOG(LogTemp, Log, TEXT("NormalAttackIcons"));
	}
	if (Icon2.Succeeded())
	{
		NormalAttackIcons[1] = Icon2.Object;
		UE_LOG(LogTemp, Log, TEXT("NormalAttackIcons"));
	}
	if (Icon3.Succeeded())
	{
		NormalAttackIcons[2] = Icon3.Object;
		UE_LOG(LogTemp, Log, TEXT("NormalAttackIcons"));
	}*/
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentDash < MaxDash)
	{
		CurrentDash = FMath::Clamp(CurrentDash + (RecoveryRate * DeltaTime), 0.0f, MaxDash);
		OnDashChanged.Broadcast(CurrentDash);
	}
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

void UPlayerStatComponent::ApplyDash()
{
	const float PrevHP = CurrentDash;
	if (CurrentDash <= 0.9f)
	{
		return;
	}
	CurrentDash--;
	SetDash(CurrentDash);
}

void UPlayerStatComponent::ChangeSkillIcon(int attacknumber)
{
	uint32 tempattacknumber = 0;
	//UE_LOG(LogTemp, Log, TEXT("컴포넌트 이름: %s | 배열 내부 개수: %d"), *GetName(), NormalAttackIcons.Num());
	//UE_LOG(LogTemp, Log, TEXT("ChangeSkillIcon: %d"), attacknumber);
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

void UPlayerStatComponent::SkillE()
{
	UE_LOG(LogTemp, Log, TEXT("SkillE"));

	FOnSkillEStart.Broadcast(coolTime_E);
}

void UPlayerStatComponent::SkillR()
{
	UE_LOG(LogTemp, Log, TEXT("SkillR"));

	FOnSkillRStart.Broadcast(coolTime_R);
}



void UPlayerStatComponent::SetDash(float NewDash)
{
	CurrentDash = FMath::Clamp<float>(NewDash, 0.0f, MaxDash);
	OnDashChanged.Broadcast(CurrentDash);
}
