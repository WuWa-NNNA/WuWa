// Fill out your copyright notice in the Description page of Project Settings.

#include "ANS_StopTimeAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "Character/Monster/Sigillum/Sigillum.h"

void UANS_StopTimeAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp || !MeshComp->GetWorld()) return;

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(MeshComp->GetWorld(), 0);
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->CustomTimeDilation = 0.0f;

		APlayerController* PC = UGameplayStatics::GetPlayerController(MeshComp->GetWorld(), 0);
		if (IsValid(PC))
		{
			PlayerCharacter->DisableInput(PC);
		}

		TArray<UActorComponent*> NiagaraComponents;
		PlayerCharacter->GetComponents(UNiagaraComponent::StaticClass(), NiagaraComponents);

		for (UActorComponent* Component : NiagaraComponents)
		{
			UNiagaraComponent* NiagaraComp = Cast<UNiagaraComponent>(Component);
			if (IsValid(NiagaraComp))
			{
				NiagaraComp->SetCustomTimeDilation(0.0f);
			}
		}
	}

	ASigillum* Boss = Cast<ASigillum>(MeshComp->GetOwner());
	if (IsValid(Boss))
	{
		Boss->OnTimeStopSpaceBegin();
	}
}

void UANS_StopTimeAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetWorld()) return;

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(MeshComp->GetWorld(), 0);
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->CustomTimeDilation = 1.0f;

		APlayerController* PC = UGameplayStatics::GetPlayerController(MeshComp->GetWorld(), 0);
		if (IsValid(PC))
		{
			PlayerCharacter->EnableInput(PC);
		}

		TArray<UActorComponent*> NiagaraComponents;
		PlayerCharacter->GetComponents(UNiagaraComponent::StaticClass(), NiagaraComponents);

		for (UActorComponent* Component : NiagaraComponents)
		{
			UNiagaraComponent* NiagaraComp = Cast<UNiagaraComponent>(Component);
			if (IsValid(NiagaraComp))
			{
				NiagaraComp->SetCustomTimeDilation(1.0f);
			}
		}
	}

	ASigillum* Boss = Cast<ASigillum>(MeshComp->GetOwner());
	if (IsValid(Boss))
	{
		Boss->OnTimeStopSpaceEnd();
	}
}