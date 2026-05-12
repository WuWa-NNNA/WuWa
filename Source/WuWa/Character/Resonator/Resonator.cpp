#include "Character/Resonator/Resonator.h"
#include "DataAsset/AttackComboData.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Stat/PlayerStatComponent.h"

AResonator::AResonator()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->GravityScale = 1.5f;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponProp05"));
	Weapon->SetVisibility(false);

	SetCurrentState(EResonatorState::Normal);
	SetCurrentLocomotionGait(ELocomotionGait::Run);

	Stat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("PlayerStat"));
	if (Stat)
	{
		UE_LOG(LogTemp, Log, TEXT("SUCCED Stat"));
	}

}

void AResonator::SetCurrentState(const EResonatorState NextState)
{
	CurrentState = NextState;
}

void AResonator::SetCurrentLocomotionGait(const ELocomotionGait NextLocomotionGait)
{
	switch (NextLocomotionGait)
	{
	case ELocomotionGait::Run:
		GetCharacterMovement()->GravityScale = 2.5f;
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		break;
	case ELocomotionGait::Sprint:
		GetCharacterMovement()->MaxWalkSpeed = 750.0f;
		break;
	}

	CurrentLocomotionGait = NextLocomotionGait;
}

void AResonator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AResonator::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AResonator::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AResonator::Jump);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AResonator::Dash);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AResonator::Attack);
	}
}

void AResonator::Tick(float DeltaSeconds)
{
	TickLocomotionGait(DeltaSeconds);
}

void AResonator::TickLocomotionGait(float DeltaSeconds)
{
	switch (CurrentLocomotionGait)
	{
	case ELocomotionGait::Sprint:
		if (GetVelocity().Length() <= 50.0f)
		{
			SetCurrentLocomotionGait(ELocomotionGait::Run);
		}
		break;
	}
}

void AResonator::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();

	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector NewMoveInputDirection = ForwardVector * Movement.Y + RightVector * Movement.X;
	if (NewMoveInputDirection.IsNearlyZero())
	{
		return;
	}

	CurrentMoveInputDirection = NewMoveInputDirection.GetSafeNormal();
	AddMovementInput(CurrentMoveInputDirection);

	CancelAttackByNewInput();
}

void AResonator::Look(const FInputActionValue& Value)
{
	FVector2D RotationValue = Value.Get<FVector2D>();

	AddControllerYawInput(RotationValue.X);
	AddControllerPitchInput(-RotationValue.Y);
}

void AResonator::Jump()
{
	if (CurrentState != EResonatorState::Normal)
	{
		return;
	}

	if (GetMovementComponent()->IsFalling() || GetMovementComponent()->IsFlying())
	{
		return;
	}

	CancelAttackByNewInput();

	if (GetVelocity().Length() > 50.0f)
	{
		PlayAnimMontage(JumpRunMontage);
	}
	else
	{
		PlayAnimMontage(JumpMontage);
	}
}

void AResonator::Dash()
{
	if (CurrentState != EResonatorState::Normal)
	{
		return;
	}

	if (CurrentLocomotionGait == ELocomotionGait::Dash)
	{
		return;
	}

	CancelAttackByNewInput();

	SetCurrentLocomotionGait(ELocomotionGait::Dash);
	SetRotationByMoveInput();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AResonator::OnDashMontageEnded);
	if (GetMovementComponent()->IsFalling() || GetMovementComponent()->IsFlying())
	{
		PlayAnimMontage(JumpDashMontage, 1.5f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, JumpDashMontage);
	}
	else if (GetVelocity().Length() > 50.0f)
	{
		PlayAnimMontage(DashMontage, 1.5f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DashMontage);
	}
	else
	{
		PlayAnimMontage(DashMontage, 1.5f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DashMontage);
		AnimInstance->Montage_JumpToSection(TEXT("Back"), DashMontage);
	}
}

void AResonator::CancelAttackByNewInput()
{
	if (!bCanCancelAttack)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->StopAllMontages(0.15f);
}

void AResonator::OnDashMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	SetCurrentLocomotionGait(ELocomotionGait::Sprint);
}

void AResonator::Attack()
{
	CancelAttackByNewInput();

	if (CurrentAttackCombo == 0)
	{
		BeginComboAttack();
		return;
	}

	if (AttackComboTimer.IsValid())
	{
		bHasNextComboCommand = true;
	}
	else
	{
		bHasNextComboCommand = false;
	}
}

void AResonator::BeginComboAttack()
{
	CurrentAttackCombo = 1;
	SetCurrentState(EResonatorState::Attack);
	SetRotationByMoveInput();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	const float AttackSpeedRate = 1.5f;
	AnimInstance->Montage_Play(AttackMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AResonator::EndComboAttack);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	SetAttackComboTimer();
}

void AResonator::EndComboAttack(UAnimMontage* TargetMontage, bool bInterrupted)
{
	CurrentAttackCombo = 0;
	SetCurrentState(EResonatorState::Normal);
}

void AResonator::SetAttackComboTimer()
{
	AttackComboTimer.Invalidate();

	const int32 ComboIndex = CurrentAttackCombo - 1;
	ensureAlways(AttackComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.5f;
	float ComboEffectTime = (AttackComboData->EffectiveFrameCount[ComboIndex] / AttackComboData->FrameRate / AttackSpeedRate);
	if (ComboEffectTime > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(AttackComboTimer, this, &AResonator::CheckAttackComboInput, ComboEffectTime, false);
	}
}

void AResonator::CheckAttackComboInput()
{
	AttackComboTimer.Invalidate();
	if (!bHasNextComboCommand)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	CurrentAttackCombo = FMath::Clamp(CurrentAttackCombo + 1, 1, AttackComboData->MaxComboCount);
	SetCurrentState(EResonatorState::Attack);
	SetRotationByMoveInput();

	FName NextSection = *FString::Printf(TEXT("%s%d"), *AttackComboData->MontageSectionNamePrefix, CurrentAttackCombo);

	AnimInstance->Montage_JumpToSection(NextSection, AttackMontage);

	SetAttackComboTimer();

	bHasNextComboCommand = false;
}

void AResonator::SetRotationByMoveInput()
{
	SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SetActorRotation(CurrentMoveInputDirection.Rotation());
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}
