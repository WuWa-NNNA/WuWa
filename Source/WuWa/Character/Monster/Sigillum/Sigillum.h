// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Monster/Monster.h"

#include "Sigillum.generated.h"

/**
 * 
 */
UCLASS()
class WUWA_API ASigillum : public AMonster
{
	GENERATED_BODY()
	
public:
	ASigillum(const FObjectInitializer& ObjectInitializer);
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	class UMaterialInterface* HiddenMaterial;
	UAnimMontage* GetParalysisMontage() const { return ParalysisMontage; }


protected:
	void ChangeToParalysis();

private: // montage;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ParalysisMontage;

private:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
