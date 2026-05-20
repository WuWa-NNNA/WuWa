// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/Monster/MonsterStatComponent.h"

void UMonsterStatComponent::showLockOn()
{
	if (!IsValid(SpawnedLockOn))
	{
		return;
	}	
	SpawnedLockOn->SetActorHiddenInGame(false);
}

void UMonsterStatComponent::hideLockOn()
{
	if (!IsValid(SpawnedLockOn))
	{
		return;
	}
	SpawnedLockOn->SetActorHiddenInGame(true);
}
