#include "Character/Resonator/Resonator.h"
#include "DataAsset/AttackComboData.h"
#include "Physics/WWCollision.h"
#include "Effect/GhostTrailEffect.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "LevelSequenceActor.h"
#include "CineCameraActor.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"

#include "Stat/Player/LunoStatComponent.h"
#include "UI/WWWidgetComponent.h"
#include "UI/UWorldUserWidget.h"
#include "PaperSprite.h"
#include "Character/Monster/Monster.h"
#include "UI/WWDashBarWidget.h"
#include "Player/WWPlayerController.h"

const float LockOnDetectRadius = 3000.0f;

AResonator::AResonator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerStatComponent>(TEXT("Stat")))
{
	TeamType = ETeamType::Resonator;

	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxFlySpeed = 900.0f;

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

	CineRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CineRoot"));
	CineRoot->SetupAttachment(RootComponent);
	CineRoot->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	CineRoot->SetRelativeRotation(FRotator::ZeroRotator);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponProp05"));
	Weapon->SetVisibility(false);

	DashGaugeComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DashGaugeComponent"));
	
	static ConstructorHelpers::FClassFinder<UUserWidget> DashBarRef(TEXT("/Game/PCH/UI/Blueprint/WBP_DashGauage.WBP_DashGauage_C"));
	if (DashBarRef.Class)
	{
		DashGaugeComponent->SetWidgetClass(DashBarRef.Class);
		DashGaugeComponent->SetWidgetSpace(EWidgetSpace::Screen);			
		DashGaugeComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool AResonator::CanConcerto()
{
	return !SequenceActor->GetSequencePlayer()->IsPlaying();
}

void AResonator::ConcertoOut()
{
	//SetActorEnableCollision(false);
	if (CurrentState == EResonatorState::Normal)
	{
		DeactivateByConcerto();
	}
}

void AResonator::ConcertoIn(AResonator* Other)
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	bIsLockOn = Other->bIsLockOn;
	LockOnTarget = Other->LockOnTarget;

	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	UPlayerStatComponent* OtherPlayerStat = Cast<UPlayerStatComponent>(Other->Stat);
	if (PlayerStat && OtherPlayerStat)
	{
		float DashValue = OtherPlayerStat->GetCurrentDash();
		PlayerStat->SetCurrentDash(DashValue);
		UpdateDashGaugeUI(DashValue);
		DashGaugeComponent->SetWorldLocation(Other->DashGaugeComponent->GetComponentLocation());
	}

	if (Other->CurrentState == EResonatorState::Normal)
	{
		SetActorTransform(Other->GetActorTransform());
	}
	else
	{
		if (bIsLockOn && LockOnTarget)
		{
			const float Radius = 250.f;
			const float RandomAngle = FMath::RandRange(0.f, 360.f);
			const FVector Offset = FRotator(0.f, RandomAngle, 0.f).Vector() * Radius;
			SetActorLocation(LockOnTarget->GetActorLocation() + Offset);
		}
		else
		{
			SetActorTransform(Other->GetActorTransform());
		}

		ConcertoAttack();
	}

	BeginConcertoGhostTrailEffect();
}

void AResonator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AResonator::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AResonator::StopMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AResonator::Look);
		EnhancedInputComponent->BindAction(LockAction, ETriggerEvent::Started, this, &AResonator::Lock);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AResonator::Jump);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AResonator::Dash);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AResonator::Attack);
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Started, this, &AResonator::Skill);
		EnhancedInputComponent->BindAction(BurstAction, ETriggerEvent::Started, this, &AResonator::Burst);
	}
}

void AResonator::InitializeCinematicActors()
{
	TArray<AActor*> FoundActors;

	if (!SequenceActor)
	{
		FoundActors.Empty();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelSequenceActor::StaticClass(), FoundActors);
		if (!FoundActors.IsEmpty() && FoundActors[0])
		{
			SequenceActor = Cast<ALevelSequenceActor>(FoundActors[0]);
		}
	}

	if (!CineCameraActor)
	{
		FoundActors.Empty();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACineCameraActor::StaticClass(), FoundActors);
		if (!FoundActors.IsEmpty() && FoundActors[0])
		{
			CineCameraActor = Cast<ACineCameraActor>(FoundActors[0]);
		}
	}

	if (!CineLookAtActor)
	{
		FoundActors.Empty();
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("CineLookAtActor"), FoundActors);
		if (!FoundActors.IsEmpty() && FoundActors[0])
		{
			CineLookAtActor = Cast<AActor>(FoundActors[0]);
		}
	}
}

void AResonator::InitializeUIComponents()
{
	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	if (!PlayerStat)
	{
		return;
	}

	PlayerStat->OnDashChanged.AddUObject(this, &AResonator::UpdateDashGaugeUI);
}

void AResonator::BeginPlay()
{
	Super::BeginPlay();

	InitializeCinematicActors();
	InitializeUIComponents();

	ChangeState(EResonatorState::Normal);
	ChangeLocomotionGait(ELocomotionGait::Run);
	DodgeTime = 0.5f;
}

void AResonator::Tick(float DeltaSeconds)
{
	TickCamera(DeltaSeconds);
	TickUIWidget(DeltaSeconds);

	switch (CurrentState)
	{
	case EResonatorState::Normal:
		TickNormal(DeltaSeconds);
		break;
	case EResonatorState::Attack:
		TickAttack(DeltaSeconds);
		break;
	}

	TickLocomotionGait(DeltaSeconds);
}

void AResonator::TickCamera(float DeltaSeconds)
{
	AController* OwingController = GetController();
	if (!OwingController)
	{
		return;
	}

	if (bApplyZMotionToCamera)
	{
		float ZOffset = GetMesh()->GetSocketLocation(TEXT("Bip001")).Z - GetActorLocation().Z - 50.0f;
		SpringArm->SocketOffset = FVector(0.0f, 0.0f, ZOffset);
	}
	else
	{
		SpringArm->SocketOffset = FVector::ZeroVector;
	}

	if (bIsLockOn && LockOnTarget)
	{
		FVector ToTarget = LockOnTarget->GetActorLocation() - GetActorLocation();
		AMonster* monster = Cast<AMonster>(LockOnTarget);
		if (ToTarget.Length() > LockOnDetectRadius || (monster && monster->bIsDead))
		{
			monster->hideLockOnMonster();

			bIsLockOn = false;
			LockOnTarget = nullptr;
		}
		else
		{
			FRotator TargetRot = ToTarget.Rotation();
			TargetRot.Roll = 0.0f;
			TargetRot.Pitch = FMath::Clamp(TargetRot.Pitch, 0.0f, 20.0f) - 20.0f;
			FRotator CurrentRot = OwingController->GetControlRotation();
			FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, 1.0f);
			OwingController->SetControlRotation(NewRot);
		}
	}
}

void AResonator::TickNormal(float DeltaSeconds)
{
	if (!GetController())
	{
		if (!IsHidden())
		{
			DeactivateByConcerto();
		}

		return;
	}

	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		bHasDashedInAir = false;
	}
}

void AResonator::TickAttack(float DeltaSeconds)
{
	if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(nullptr))
	{
		CurrentAttackCombo = 0;
		UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
		PlayerStat->ChangeSkillIcon(CurrentAttackCombo);
		ChangeState(EResonatorState::Normal);
		return;
	}
}

void AResonator::TickLocomotionGait(float DeltaSeconds)
{
	switch (CurrentLocomotionGait)
	{
	case ELocomotionGait::Sprint:
		if (!bHasCurrentMoveInput)
		{
			ChangeLocomotionGait(ELocomotionGait::Run);
		}
		break;
	}
}

void AResonator::TickUIWidget(float DeltaSeconds)
{
	CurrentLocation = DashGaugeComponent->GetComponentLocation();
	TargetLocation = GetActorLocation() + Camera->GetRightVector() * 70.0f + Camera->GetUpVector() * -30.0f;
	NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaSeconds, 8.0f);
	DashGaugeComponent->SetWorldLocation(NewLocation);
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

void AResonator::DeactivateByConcerto()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		AnimInst->StopAllMontages(0.0f);
		Weapon->GetAnimInstance()->StopAllMontages(0.0f);
	}

	BeginConcertoGhostTrailEffect();
}

void AResonator::SetCameraLag(bool bNewValue)
{
	SpringArm->bEnableCameraLag = bNewValue;
	SpringArm->bEnableCameraRotationLag = bNewValue;
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

	bHasCurrentMoveInput = true;
	CurrentMoveInputDirection = NewMoveInputDirection.GetSafeNormal();

	if (CurrentState != EResonatorState::Normal)
	{
		return;
	}

	TryCancelAttackMontageByNewInput();

	AddMovementInput(CurrentMoveInputDirection);
}

void AResonator::StopMove(const FInputActionValue& Value)
{
	bHasCurrentMoveInput = false;
}

void AResonator::Look(const FInputActionValue& Value)
{
	if (bIsLockOn)
	{
		return;
	}

	FVector2D RotationValue = Value.Get<FVector2D>();

	AddControllerYawInput(RotationValue.X);
	AddControllerPitchInput(-RotationValue.Y);
}

void AResonator::Lock()
{
	LockOnTarget = nullptr;

	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	bool bHitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, GetActorLocation(), GetActorLocation(), FQuat::Identity, CCHANNEL_WWACTION, FCollisionShape::MakeSphere(LockOnDetectRadius), Params);

	if (!bHitDetected)
	{
		bIsLockOn = false;
		if (AMonster* monster = Cast<AMonster>(LockOnTarget))
		{
			monster->hideLockOnMonster();
		}

		return;
	}

	ICombatTeamInterface* MyTeamActor = Cast<ICombatTeamInterface>(this);
	if (!MyTeamActor)
	{
		bIsLockOn = false;
		if (AMonster* monster = Cast<AMonster>(LockOnTarget))
		{
			monster->hideLockOnMonster();
		}

		return;
	}

	for (const FHitResult& HitResult : OutHitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!IsValid(HitActor))
		{
			continue;
		}

		ICombatTeamInterface* HitTeamActor = Cast<ICombatTeamInterface>(HitActor);
		if (!HitTeamActor)
		{
			continue;
		}

		if (HitTeamActor->GetTeamType() == MyTeamActor->GetTeamType())
		{
			continue;
		}

		LockOnTarget = HitActor;
		break;
	}

	bIsLockOn = !bIsLockOn;
	if (bIsLockOn && LockOnTarget)
	{
		if (AMonster* monster = Cast<AMonster>(LockOnTarget))
		{
			monster->showLockOnMonster();
			UPlayerStatComponent* playerStat = Cast< UPlayerStatComponent>(Stat);
			playerStat->LockOnUI();
		}
	}
	else
	{
		if (AMonster* monster = Cast<AMonster>(LockOnTarget))
		{
			monster->hideLockOnMonster();
		}

		LockOnTarget = nullptr;
	}
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

	if (bHasCurrentMoveInput)
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
	bHasCurrentDashInput = true;
	GetWorldTimerManager().ClearTimer(DodgeTimer);
	GetWorldTimerManager().SetTimer(DodgeTimer, this, &AResonator::OnFinishedDodgeTimer, DodgeTime, false);

	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	if (!PlayerStat || PlayerStat->GetCurrentDash() <= 0.75f)
	{
		return;
	}

	if (CurrentState != EResonatorState::Normal)
	{
		return;
	}

	if (CurrentState == EResonatorState::Normal && CurrentLocomotionGait == ELocomotionGait::Dash)
	{
		return;
	}

	if (GetMovementComponent()->IsFalling() || GetMovementComponent()->IsFlying())
	{
		if (!CanAirDash())
		{
			return;
		}

		bHasDashedInAir = true;
	}

	TryCancelAttackMontageByNewInput();

	ChangeLocomotionGait(ELocomotionGait::Dash);
	SetRotationByMoveInput();

	PlayDashMontage();

	UseDashGauge();
}

void AResonator::Dodge()
{
	bHasCurrentDashInput = false;
	GetWorldTimerManager().ClearTimer(DodgeTimer);

	ChangeState(EResonatorState::Dodge);
	PlayDodgeMontage();
	
	SpawnGhostTrailEffect();
	GetWorldTimerManager().SetTimer(GhostTrailEffectSpawnTimer, this, &AResonator::SpawnGhostTrailEffect, 0.05f, true);

	UseDashGauge();
}

void AResonator::Attack()
{
	if (GetMovementComponent()->IsFalling() || GetMovementComponent()->IsFlying())
	{
		return;
	}

	ProcessAttack();
}

void AResonator::Skill()
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

	ChangeState(EResonatorState::Attack);	
	SetAttackRotationByMoveInput();

	PlayAnimMontage(SkillMontage, 1.5f);
	Weapon->GetAnimInstance()->Montage_Play(WeaponSkillMontage, 1.5f);

	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->SkillE();
}

void AResonator::Burst()
{
	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	if (!PlayerStat->IsRPossible())
	{
		return;
	}

	if (CurrentState != EResonatorState::Normal)
	{
		return;
	}

	InitializeCinematicActors();
	if (!SequenceActor || !CineCameraActor || !CineLookAtActor)
	{
		return;
	}

	TryCancelAttackMontageByNewInput();

	ChangeState(EResonatorState::Attack);
	SetAttackRotationByMoveInput();

	PlayAnimMontage(BurstMontage, 1.0f);
	Weapon->GetAnimInstance()->Montage_Play(WeaponBurstMontage, 1.0f);
	PlayBurstCinematic();
	PlayerStat->SetRGauge(0.0f);
}

void AResonator::ConcertoAttack()
{
	ChangeState(EResonatorState::Attack);
	SetAttackRotationByMoveInput();

	PlayAnimMontage(ConcertoAttackMontage, 1.5f);
	Weapon->GetAnimInstance()->Montage_Play(ConcertoAttackMontage, 1.5f);
}

void AResonator::OnAttackSucceeded(TSet<TObjectPtr<AActor>>& DamagedActors, AActor* HitActor, const FHitResult& HitResult, bool& bDidShakeCamera)
{
	Super::OnAttackSucceeded(DamagedActors, HitActor, HitResult, bDidShakeCamera);

	RGaugeUp();
}

float AResonator::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CurrentState == EResonatorState::Dodge)
	{
		return 0.0f;
	}

	if (bHasCurrentDashInput)
	{
		Dodge();
		return 0.0f;
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return ActualDamage;
}

void AResonator::ProcessAttack()
{
	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->ChangeSkillIcon(CurrentAttackCombo);

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

	CurrentAttackCombo = 0;
	bHasNextComboCommand = true;
	CheckAttackComboInput();
}

bool AResonator::CanAirDash()
{
	return !bHasDashedInAir;
}

void AResonator::PlayDashMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AResonator::OnDashMontageEnded);
	if (GetMovementComponent()->IsFalling() || GetMovementComponent()->IsFlying())
	{
		if (bHasCurrentMoveInput)
		{
			PlayAnimMontage(JumpDashMontage, 1.5f);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, JumpDashMontage);
		}
		else
		{
			PlayAnimMontage(JumpDashMontage, 1.5f);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, JumpDashMontage);
			AnimInstance->Montage_JumpToSection(TEXT("Back"), JumpDashMontage);
		}
	}
	else
	{
		if (bHasCurrentMoveInput)
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
}

void AResonator::PlayDodgeMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AResonator::OnDodgeMontageEnded);
	if (bHasCurrentMoveInput)
	{
		PlayAnimMontage(DodgeMontage, 1.5f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeMontage);
		GetWeaponMeshComponent()->GetAnimInstance()->StopAllMontages(0.0f);
	}
	else
	{
		PlayAnimMontage(DodgeMontage, 1.5f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeMontage);
		AnimInstance->Montage_JumpToSection(TEXT("Back"), DodgeMontage);
		GetWeaponMeshComponent()->GetAnimInstance()->StopAllMontages(0.0f);
	}
}

void AResonator::PlayBurstCinematic()
{
	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->HideUI();

	CineCameraActor->AttachToComponent(CineRoot, FAttachmentTransformRules::KeepRelativeTransform);
	CineLookAtActor->AttachToComponent(CineRoot, FAttachmentTransformRules::KeepRelativeTransform);
	Cast<APlayerController>(GetController())->SetViewTargetWithBlend(CineCameraActor, 0.0f);

	SequenceActor->SetSequence(BurstSequence);
	SequenceActor->SetBindingByTag(FName("CineCameraActor"), { CineCameraActor }, true);
	SequenceActor->SetBindingByTag(FName("CineLookAtActor"), { CineLookAtActor }, true);

	ULevelSequencePlayer* SequencePlayer = SequenceActor->GetSequencePlayer();
	SequencePlayer->OnFinished.AddUniqueDynamic(this, &AResonator::OnBurstCinematicEnded);
	SequencePlayer->SetPlayRate(1.0f);
	SequencePlayer->Play();
}

void AResonator::SetRotationByMoveInput()
{
	if (!bHasCurrentMoveInput)
	{
		return;
	}

	SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SetActorRotation(CurrentMoveInputDirection.Rotation());
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

void AResonator::SetAttackRotationByMoveInput()
{
	if (!bIsLockOn || !LockOnTarget)
	{
		SetRotationByMoveInput();
		return;
	}

	FVector ToTarget = LockOnTarget->GetActorLocation() - GetActorLocation();
	FRotator TargetRot = ToTarget.Rotation();
	TargetRot.Roll = 0.0f;
	TargetRot.Pitch = 0.0f;

	SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SetActorRotation(TargetRot);
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

void AResonator::TryCancelAttackMontageByNewInput()
{
	if (CurrentState == EResonatorState::Attack || !bCanCancelAttack)
	{
		return;
	}

	GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
	Weapon->GetAnimInstance()->StopAllMontages(0.0f);

	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->ChangeSkillIcon(0);
}

void AResonator::OnFinishedDodgeTimer()
{
	bHasCurrentDashInput = false;
}

void AResonator::SpawnGhostTrailEffect()
{
	if (!GhostTrailEffectClass || !GetMesh())
	{
		return;
	}

	AGhostTrailEffect* Ghost = GetWorld()->SpawnActor<AGhostTrailEffect>(GhostTrailEffectClass, GetMesh()->GetComponentTransform());
	if (Ghost)
	{
		Ghost->Initialize(GetMesh(), GhostTrailEffectMaterial, 0.3f);
	}
}

void AResonator::BeginComboAttack()
{
	CurrentAttackCombo = 1;
	ChangeState(EResonatorState::Attack);
	SetAttackRotationByMoveInput();

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AResonator::OnAttackMontageEnded);
	const float AttackSpeedRate = 1.5f;
	PlayAnimMontage(AttackMontage, AttackSpeedRate);
	Weapon->GetAnimInstance()->Montage_Play(WeaponAttackMontage, AttackSpeedRate);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	SetAttackComboTimer();
	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->PlaySkillIconAnimation();
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
		GetWorldTimerManager().ClearTimer(AttackComboTimer);
		GetWorldTimerManager().SetTimer(AttackComboTimer, this, &AResonator::CheckAttackComboInput, ComboEffectTime, false);
	}
}

void AResonator::CheckAttackComboInput()
{
	AttackComboTimer.Invalidate();
	if (!bHasNextComboCommand)
	{
		UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
		PlayerStat->ChangeSkillIcon(0);
		return;
	}

	if (++CurrentAttackCombo > AttackComboData->MaxComboCount)
	{
		CurrentAttackCombo = 1;
	}

	ChangeState(EResonatorState::Attack);
	SetAttackRotationByMoveInput();

	FName NextSection = *FString::Printf(TEXT("%s%d"), *AttackComboData->MontageSectionNamePrefix, CurrentAttackCombo);
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(NextSection, AttackMontage);
	Weapon->GetAnimInstance()->Montage_JumpToSection(NextSection, WeaponAttackMontage);

	SetAttackComboTimer();

	bHasNextComboCommand = false;

	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->ChangeSkillIcon(CurrentAttackCombo);
	PlayerStat->PlaySkillIconAnimation();
}

void AResonator::BeginConcertoGhostTrailEffect()
{
	SpawnGhostTrailEffect();
	GetWorldTimerManager().ClearTimer(GhostTrailEffectSpawnTimer);
	GetWorldTimerManager().SetTimer(GhostTrailEffectSpawnTimer, this, &AResonator::SpawnGhostTrailEffect, 0.05f, true);
	GetWorldTimerManager().ClearTimer(ConcertoBlendTimer);
	GetWorldTimerManager().SetTimer(ConcertoBlendTimer, this, &AResonator::OnConcertoBlendEnded, 0.3f);
}

void AResonator::OnConcertoBlendEnded()
{
	GetWorldTimerManager().ClearTimer(GhostTrailEffectSpawnTimer);
}

void AResonator::OnDashMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ChangeLocomotionGait(ELocomotionGait::Sprint);
}

void AResonator::OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ChangeState(EResonatorState::Normal);
	GetWorldTimerManager().ClearTimer(GhostTrailEffectSpawnTimer);
}

void AResonator::OnAttackMontageEnded(UAnimMontage* TargetMontage, bool bInterrupted)
{
	CurrentAttackCombo = 0;
	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->ChangeSkillIcon(CurrentAttackCombo);
}

void AResonator::OnBurstCinematicEnded()
{
	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->ShowUI();
	Cast<APlayerController>(GetController())->SetViewTargetWithBlend(this, 0.0f);
}

void AResonator::DamagedTest()
{
	Stat->ApplyDamage(2);
}

void AResonator::RGaugeUp()
{
	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->SetRGauge(PlayerStat->GetRGauge() + 0.05f);
}

void AResonator::OpenUI()
{
	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->ShowUI();
}

void AResonator::CloseUI()
{
	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->HideUI();
}

void AResonator::UseDashGauge()
{
	if (UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat))
	{
		PlayerStat->ApplyDash();
	}
}

void AResonator::UpdateDashGaugeUI(float NewDash)
{
	if (!DashGaugeComponent)
	{
		return;
	}

	UWWDashBarWidget* DashWidget = Cast<UWWDashBarWidget>(DashGaugeComponent->GetUserWidgetObject());

	if (!DashWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("DashWidget NULL"));
		return;
	}

	if (GetController() && !SequenceActor->GetSequencePlayer()->IsPlaying())
	{
		DashWidget->UpdataWorldDash(NewDash);
	}
	else
	{
		DashWidget->HideWorldDash();
	}
}
