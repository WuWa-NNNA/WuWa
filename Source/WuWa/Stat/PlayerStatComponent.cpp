// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/PlayerStatComponent.h"
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
	NormalAttackIcons.SetNum(3);

	static ConstructorHelpers::FObjectFinder<UPaperSprite> Icon1(TEXT("/Game/PCH/Asset/UIImage/YounuoSkill/LB1.LB1"));
	static ConstructorHelpers::FObjectFinder<UPaperSprite> Icon2(TEXT("/Game/PCH/Asset/UIImage/YounuoSkill/LB2.LB2"));
	static ConstructorHelpers::FObjectFinder<UPaperSprite> Icon3(TEXT("/Game/PCH/Asset/UIImage/YounuoSkill/LB3.LB3"));


	if (Icon1.Succeeded())
	{
		NormalAttackIcons[0] = Icon1.Object;
		//NormalAttackIcons.Add(Icon1.Object);
		UE_LOG(LogTemp, Log, TEXT("NormalAttackIcons"));

	}
	if (Icon2.Succeeded())
	{
		NormalAttackIcons[1] = Icon2.Object;

		//NormalAttackIcons.Add(Icon2.Object);
		UE_LOG(LogTemp, Log, TEXT("NormalAttackIcons"));
	}
	if (Icon3.Succeeded())
	{
		NormalAttackIcons[2] = Icon3.Object;

		//NormalAttackIcons.Add(Icon3.Object);
		UE_LOG(LogTemp, Log, TEXT("NormalAttackIcons"));
	}
}

// Called when the game starts
void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
	//SetDash(MaxDash);
	
	auto DefaultObject = GetClass()->GetDefaultObject();
	UE_LOG(LogTemp, Log, TEXT("Test"));
}

void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//UE_LOG(LogTemp, Log, TEXT("no change : %f"), GetCurrentDash());

	if (CurrentDash < MaxDash)
	{
		CurrentDash = FMath::Clamp(CurrentDash + (RecoveryRate * DeltaTime), 0.0f, MaxDash);
		OnDashChanged.Broadcast(CurrentDash);
	}
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

void UPlayerStatComponent::ChangeSkillIcon(float attacknumber)
{
	int32 Index = FMath::FloorToInt(attacknumber-1);
	if (NormalAttackIcons.IsValidIndex(Index))
	{
		UPaperSprite* SelectedIcon = NormalAttackIcons[Index];
		if (SelectedIcon)
		{
			OnBaseSkillchange.Broadcast(SelectedIcon);
		}
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
	UE_LOG(LogTemp, Log, TEXT("Dash Count : %f"), CurrentDash);

	CurrentDash = FMath::Clamp<float>(NewDash, 0.0f, MaxDash);
	OnDashChanged.Broadcast(CurrentDash);
}
