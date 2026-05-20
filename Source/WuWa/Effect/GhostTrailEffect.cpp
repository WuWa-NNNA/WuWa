#include "Effect/GhostTrailEffect.h"

#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AGhostTrailEffect::AGhostTrailEffect()
{
	PrimaryActorTick.bCanEverTick = true;

    GhostMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("GhostMesh"));
    SetRootComponent(GhostMesh);

    SetActorEnableCollision(false);
}

void AGhostTrailEffect::Initialize(USkeletalMeshComponent* SourceMesh, UMaterialInterface* InGhostMaterial, float InLifeTime)
{
    if (!SourceMesh)
    {
        Destroy();
        return;
    }

    LifeTime = InLifeTime;
    ElapsedTime = 0.f;

    GhostMesh->SetSkeletalMesh(SourceMesh->GetSkeletalMeshAsset());
    GhostMesh->SetWorldTransform(SourceMesh->GetComponentTransform());
    GhostMesh->CopyPoseFromSkeletalComponent(SourceMesh);
    GhostMesh->SetVisibleInRayTracing(false);

    DynamicMaterials.Empty();

    const int32 MaterialCount = SourceMesh->GetNumMaterials();
    for (int32 i = 0; i < MaterialCount; ++i)
    {
        UMaterialInterface* BaseMaterial = InGhostMaterial ? InGhostMaterial : SourceMesh->GetMaterial(i);

        UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BaseMaterial, this);
        GhostMesh->SetMaterial(i, MID);

        DynamicMaterials.Add(MID);
    }
}

void AGhostTrailEffect::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ElapsedTime += DeltaTime;

    const float Alpha = FMath::Clamp(1.f - ElapsedTime / LifeTime, 0.f, 1.f);

    for (UMaterialInstanceDynamic* MID : DynamicMaterials)
    {
        if (MID)
        {
            MID->SetScalarParameterValue(TEXT("Opacity"), Alpha);
        }
    }

    if (ElapsedTime >= LifeTime)
    {
        Destroy();
    }
}
