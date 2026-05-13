#include "Character/Resonator/Resonator.h"
#include "DataAsset/AttackComboData.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"

// Stat/UI
#include "Stat/PlayerStatComponent.h"
#include "UI/WWWidgetComponent.h"
#include "Components/WidgetComponent.h"

AResonator::AResonator()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
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
		UE_LOG(LogTemp, Log, TEXT("SUCCED Stat!"));
	}
	DashBar = CreateDefaultSubobject<UWWWidgetComponent>(TEXT("HPWidget"));
	DashBar->SetupAttachment(GetMesh());

	DashBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> DashWidgetRef(TEXT("/Game/PCH/UI/Blueprint/WBP_Dash.WBP_Dash_C"));
	if (DashWidgetRef.Succeeded())
	{
		DashBar->SetWidgetClass(DashWidgetRef.Class);
		DashBar->SetWidgetSpace(EWidgetSpace::Screen);
		DashBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		DashBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);;
	}
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
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AResonator::Attack);
		EnhancedInputComponent->BindAction(SAttackAction, ETriggerEvent::Started, this, &AResonator::SAttack);
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Started, this, &AResonator::Skill);
	}
}

void AResonator::BeginPlay()
{
	Super::BeginPlay();

	ChangeState(EResonatorState::Normal);
	ChangeLocomotionGait(ELocomotionGait::Run);
}

void AResonator::Tick(float DeltaSeconds)
{
	if (bApplyZMotionToCamera)
	{
		float ZOffset = GetMesh()->GetSocketLocation(TEXT("Bip001")).Z - GetActorLocation().Z - 50.0f;
		SpringArm->SocketOffset = FVector(0.0f, 0.0f, ZOffset);
	}
	else
	{
		SpringArm->SocketOffset = FVector::Zero();
	}

	TickLocomotionGait(DeltaSeconds);
}

void AResonator::TickLocomotionGait(float DeltaSeconds)
{
	switch (CurrentLocomotionGait)
	{
	case ELocomotionGait::Sprint:
		if (GetVelocity().Length() <= 50.0f)
		{
			ChangeLocomotionGait(ELocomotionGait::Run);
		}
		break;
	}
}

void AResonator::ChangeState(const EResonatorState NextState)
{
	CurrentState = NextState;
}

void AResonator::ChangeLocomotionGait(const ELocomotionGait NextLocomotionGait)
{
	switch (NextLocomotionGait)
	{
	case ELocomotionGait::Run:
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		break;
	case ELocomotionGait::Sprint:
		GetCharacterMovement()->MaxWalkSpeed = 750.0f;
		break;
	}

	CurrentLocomotionGait = NextLocomotionGait;
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

	if (CurrentState != EResonatorState::Normal)
	{
		return;
	}

	TryCancelAttackMontageByNewInput();

	AddMovementInput(CurrentMoveInputDirection);
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

	TryCancelAttackMontageByNewInput();

	if (GetVelocity().Length() > 50.0f)
	{
		SetRotationByMoveInput();
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

	TryCancelAttackMontageByNewInput();

	ChangeLocomotionGait(ELocomotionGait::Dash);
	SetRotationByMoveInput();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

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

void AResonator::Attack()
{
	if (CurrentAttackCombo == 0)
	{
		if (CurrentState != EResonatorState::Normal)
		{
			return;
		}

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

	if (CurrentState == EResonatorState::Attack || !bCanCancelAttack)
	{
		return;
	}

	bHasNextComboCommand = true;
	CheckAttackComboInput();
}

void AResonator::SAttack()
{
	if (CurrentState != EResonatorState::Normal)
	{
		return;
	}

	TryCancelAttackMontageByNewInput();
	
	//ChangeState(EResonatorState::Attack);
	//SetRotationByMoveInput();
	//
	//PlayAnimMontage(SAttackMontage, 1.5f);
}

void AResonator::Skill()
{
	if (CurrentState != EResonatorState::Normal)
	{
		return;
	}

	TryCancelAttackMontageByNewInput();

	ChangeState(EResonatorState::Attack);
	SetRotationByMoveInput();

	PlayAnimMontage(SkillMontage, 1.5f);
}

void AResonator::SetRotationByMoveInput()
{
	SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SetActorRotation(CurrentMoveInputDirection.Rotation());
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

void AResonator::TryCancelAttackMontageByNewInput()
{
	if (CurrentState == EResonatorState::Attack || !bCanCancelAttack)
	{
		return;
	}

	GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
}

void AResonator::BeginComboAttack()
{
	CurrentAttackCombo = 1;
	ChangeState(EResonatorState::Attack);
	SetRotationByMoveInput();

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AResonator::OnAttackMontageEnded);
	const float AttackSpeedRate = 1.5f;
	PlayAnimMontage(AttackMontage, AttackSpeedRate);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	SetAttackComboTimer();
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

	if (++CurrentAttackCombo > AttackComboData->MaxComboCount)
	{
		CurrentAttackCombo = 1;
	}

	ChangeState(EResonatorState::Attack);
	SetRotationByMoveInput();

	FName NextSection = *FString::Printf(TEXT("%s%d"), *AttackComboData->MontageSectionNamePrefix, CurrentAttackCombo);
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(NextSection, AttackMontage);

	SetAttackComboTimer();

	bHasNextComboCommand = false;
}

void AResonator::OnDashMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ChangeLocomotionGait(ELocomotionGait::Sprint);
}

void AResonator::OnAttackMontageEnded(UAnimMontage* TargetMontage, bool bInterrupted)
{
	CurrentAttackCombo = 0;
}
