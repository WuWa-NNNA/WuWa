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

    ACharacter* Character = Cast<ACharacter>(Pawn);
    if (!Character)
    {
        return;
    }

    Speed = Pawn->GetVelocity().Length();

    bIsInAir = Character->GetMovementComponent()->IsFalling() || Character->GetMovementComponent()->IsFlying();
}
