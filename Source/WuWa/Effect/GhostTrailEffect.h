#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostTrailEffect.generated.h"

UCLASS()
class WUWA_API AGhostTrailEffect : public AActor
{
	GENERATED_BODY()
	
public:
	AGhostTrailEffect();

public:
	void Initialize(USkeletalMeshComponent* SourceMesh, UMaterialInterface* InGhostMaterial, float InLifeTime = 0.4f);

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPoseableMeshComponent> GhostMesh;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;

	float LifeTime = 0.4f;
	float ElapsedTime = 0.f;
};
