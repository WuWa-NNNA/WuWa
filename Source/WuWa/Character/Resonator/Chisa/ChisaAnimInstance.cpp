#include "Character/Resonator/Chisa/ChisaAnimInstance.h"
#include "Character/Resonator/Chisa/Chisa.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UChisaAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    APawn* Pawn = TryGetPawnOwner();
    if (!Pawn)
    {
        return;
    }

    Speed = Pawn->GetVelocity().Length();

    ACharacter* Character = Cast<ACharacter>(Pawn);
    if (!Character)
    {
        return;
    }

    bIsInAir = Character->GetMovementComponent()->IsFalling() || Character->GetMovementComponent()->IsFlying();

    AResonator* Resonator = Cast<AResonator>(Character);
    if (!Resonator)
    {
        return;
    }

    CurrentLocomotionGait = Resonator->GetCurrentLocomotionGait();
}
