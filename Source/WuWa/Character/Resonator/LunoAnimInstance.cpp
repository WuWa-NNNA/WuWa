#include "Character/Resonator/LunoAnimInstance.h"
#include "Character/Resonator/Luno.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void ULunoAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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
