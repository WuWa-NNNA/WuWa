#include "Character/Resonator/Resonator.h"
#include "DataAsset/AttackComboData.h"
#include "Physics/WWCollision.h"

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

// Stat/UI
#include "Stat/Player/LunoStatComponent.h"
#include "UI/WWWidgetComponent.h"
#include "UI/UWorldUserWidget.h"
#include "PaperSprite.h"
#include "Character/Monster/Monster.h"

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

	/*MainHUD = CreateDefaultSubobject<UUWorldUserWidget>(TEXT("MainHUD"));

	if (!MainHUD)
	{
		UE_LOG(LogTemp, Log, TEXT("Fail MainHUD"));
	}*/
	//static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetAsset(TEXT("/Game/PCH/UI/Blueprint/WBP_HUD.WBP_HUD_C"));
	/*if (HUDWidgetAsset.Succeeded())
	{
		HUDWidgetClass = HUDWidgetAsset.Class;
	}*/
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

void AResonator::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelSequenceActor::StaticClass(), FoundActors);
	if (!FoundActors.IsEmpty() && FoundActors[0])
	{
		SequenceActor = Cast<ALevelSequenceActor>(FoundActors[0]);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACineCameraActor::StaticClass(), FoundActors);
	if (!FoundActors.IsEmpty() && FoundActors[0])
	{
		CineCameraActor = Cast<ACineCameraActor>(FoundActors[0]);
	}
	
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("CineLookAtActor"), FoundActors);
	if (!FoundActors.IsEmpty() && FoundActors[0])
	{
		CineLookAtActor = Cast<AActor>(FoundActors[0]);
	}

	ChangeState(EResonatorState::Normal);
	ChangeLocomotionGait(ELocomotionGait::Run);

	/*if (HUDWidgetClass)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			MainHUD = CreateWidget<UUWorldUserWidget>(PC, HUDWidgetClass);
			if (MainHUD)
			{
				MainHUD->AddToViewport();
				MainHUD->SetOwningActor(this);

				if (Stat)
				{
					UPlayerStatComponent* PlayerStat123Wideget = Cast<UPlayerStatComponent>(Stat);

					PlayerStat123Wideget->OnHpChagned.AddUObject(MainHUD, &UUWorldUserWidget::UpdateHpBar);
					PlayerStat123Wideget->OnDashChanged.AddUObject(MainHUD, &UUWorldUserWidget::UpdateMainHUD);

					MainHUD->UpdateHpBar(Stat->GetCurrentHP());
					MainHUD->UpdateMainHUD(PlayerStat123Wideget->GetCurrentDash());
					MainHUD->UpdateLevel(Stat->GetLevel());

					PlayerStat123Wideget->ChangeSkillIcon(CurrentAttackCombo);
				}
			}
		}
	}*/
	  
}

void AResonator::Tick(float DeltaSeconds)
{
	TickCamera(DeltaSeconds);
	switch (CurrentState)
	{
	case EResonatorState::Attack:
		TickAttack(DeltaSeconds);
		break;
	}
	TickLocomotionGait(DeltaSeconds);
}

void AResonator::TickCamera(float DeltaSeconds)
{
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
		FRotator TargetRot = ToTarget.Rotation();
		TargetRot.Roll = 0.0f;
		TargetRot.Pitch = FMath::Clamp(TargetRot.Pitch, 0.0f, 20.0f) - 20.0f;

		FRotator CurrentRot = GetController()->GetControlRotation();
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, 1.0f);

		GetController()->SetControlRotation(NewRot);
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
	TArray<FHitResult> OutHitResults;

	const float DetectRadius = 3000.0f;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	FVector Start = GetActorLocation();
	FVector End = Start;

	bool bHitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, CCHANNEL_WWACTION, FCollisionShape::MakeSphere(DetectRadius), Params);

	if (!bHitDetected)
	{
		return;
	}

	bIsLockOn = !bIsLockOn;
	if (bIsLockOn)
	{
		LockOnTarget = OutHitResults[0].GetActor();
		AMonster* monster = Cast<AMonster>(LockOnTarget);
		if (monster)
		{
			monster->showLockOnMonster();
		}
	}
	else
	{
		AMonster* monster = Cast<AMonster>(LockOnTarget);
		if (monster)
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
	if (CurrentState != EResonatorState::Normal)
	{
		return;
	}

	if (CurrentLocomotionGait == ELocomotionGait::Dash)
	{
		return;
	}

	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	if (!PlayerStat)
	{
		return;
	}
	if (PlayerStat->GetCurrentDash() <= 1.f)
	{
		return;
	}

	PlayerStat->ApplyDash();

	TryCancelAttackMontageByNewInput();

	ChangeLocomotionGait(ELocomotionGait::Dash);
	SetRotationByMoveInput();

	PlayDashMontage();
}

void AResonator::DamagedTest()
{
	Stat->ApplyDamage(2);
}

void AResonator::BurstTest()
{
	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->SkillR();
}

void AResonator::RGaugeUp()
{
	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->SetRGauge(PlayerStat->GetRGauge() + 0.2f);
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
	Weapon->GetAnimInstance()->StopAllMontages(0.0f);

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

float AResonator::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{


	return 0.0f;
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

		RGaugeUp();
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

void AResonator::PlayBurstCinematic()
{
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
	PlayerStat->ChangeSkillIcon(CurrentAttackCombo);
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
	
	RGaugeUp();

	ChangeState(EResonatorState::Attack);
	SetAttackRotationByMoveInput();

	FName NextSection = *FString::Printf(TEXT("%s%d"), *AttackComboData->MontageSectionNamePrefix, CurrentAttackCombo);
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(NextSection, AttackMontage);
	Weapon->GetAnimInstance()->Montage_JumpToSection(NextSection, WeaponAttackMontage);

	SetAttackComboTimer();

	bHasNextComboCommand = false;

	UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	PlayerStat->ChangeSkillIcon(CurrentAttackCombo);
}

void AResonator::OnDashMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ChangeLocomotionGait(ELocomotionGait::Sprint);
}

void AResonator::OnAttackMontageEnded(UAnimMontage* TargetMontage, bool bInterrupted)
{
	CurrentAttackCombo = 0;
}

void AResonator::OnBurstCinematicEnded()
{
	Cast<APlayerController>(GetController())->SetViewTargetWithBlend(this, 0.0f);
}

void AResonator::SetupCharacterWidget(UWWUserWidget* InUserWidget)
{
	//UPlayerStatComponent* PlayerStat = Cast<UPlayerStatComponent>(Stat);
	//UUWorldUserWidget* MyWidget = Cast<UUWorldUserWidget>(InUserWidget);

	//if (PlayerStat && MyWidget)
	//{
	//	MyWidget->SetMaxHp(Stat->GetMaxHP());
	//	MyWidget->SetMaxDash(PlayerStat->GetMaxDash());

	//	MyWidget->UpdateHpBar(Stat->GetCurrentHP());
	//	MyWidget->UpdateMainHUD(PlayerStat->GetCurrentDash());


	//	PlayerStat->OnHpChagned.AddUObject(MyWidget, &UUWorldUserWidget::UpdateHpBar);
	//	PlayerStat->OnDashChanged.AddUObject(MyWidget, &UUWorldUserWidget::UpdateMainHUD);
	//	PlayerStat->FOnSkillEStart.AddUObject(MyWidget, &UUWorldUserWidget::SkillCoolEActive);

	//	PlayerStat->FOnSkillRStart.AddUObject(MyWidget, &UUWorldUserWidget::UpdateSkillCoolR);
	//	PlayerStat->OnBaseSkillchange.AddUObject(MyWidget, &UUWorldUserWidget::UpdateSkillIcon);
	//

	//}
}
