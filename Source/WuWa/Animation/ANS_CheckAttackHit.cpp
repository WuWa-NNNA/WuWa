#include "Animation/ANS_CheckAttackHit.h"

void UANS_CheckAttackHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	DamagedActors.Empty();
}

void UANS_CheckAttackHit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	IAttackAnimNotifyInterface* AttackPawn = Cast<IAttackAnimNotifyInterface>(MeshComp->GetOwner());
	if (AttackPawn)
	{
		AttackPawn->CheckAttackHit(AttackHitData, DamagedActors);
	}
}

void UANS_CheckAttackHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	DamagedActors.Empty();
}
