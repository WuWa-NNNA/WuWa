// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WWUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API UWWUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	//외부에서 이 UI를 A의 것이라고 설정해주는 함수 
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwningActor = NewOwner; }

protected :
	// 관리할 대상을 가리키는 변수 어던 캐릭터의 것인지 그 정보를 저장하는 저장소
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Actor)
	TObjectPtr<AActor> OwningActor;
};
