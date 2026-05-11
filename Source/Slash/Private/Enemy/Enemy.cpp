

#include "Enemy/Enemy.h"
#include "AIController.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

#include "HUD/HealthBarComponent.h"
#include "Components/AttributeComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Slash/DebugMacros.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	GetMesh()->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_Visibility, 
		ECollisionResponse::ECR_Block
	);
	GetMesh()->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_Camera,
		ECollisionResponse::ECR_Ignore
	);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_Camera,
		ECollisionResponse::ECR_Ignore
	);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	EnemySensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Enemy Sensing"));
	EnemySensing->SightRadius = 4000.f;
	EnemySensing->SetPeripheralVisionAngle(45.f);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidget) { HealthBarWidget->SetVisibility(false); }

	EnemyController = Cast<AAIController>(GetController());

	MoveToTarget(PatrolTarget);

	if (EnemySensing)
		EnemySensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
	
}

/**************************************************************************************/

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
	
}

void AEnemy::Die()
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 Selection = FMath::RandRange(0, 5);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			SectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			SectionName = FName("Death5");
			DeathPose = EDeathPose::EDP_Death5;
			break;
		case 5:
			SectionName = FName("Death6");
			DeathPose = EDeathPose::EDP_Death6;
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	if (HealthBarWidget) { HealthBarWidget->SetVisibility(false); }
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);
}

/**************************************************************************************/

void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		// Outside comhat radius, lose interest
		CombatTarget = nullptr;

		if (HealthBarWidget)
			HealthBarWidget->SetVisibility(false);

		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToTarget(PatrolTarget);
		UE_LOG(LogTemp, Warning, TEXT("Lose interest"));
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		// Outside attack range, chase character
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
		UE_LOG(LogTemp, Warning, TEXT("Chase Player"));
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		// Inside attack range, attack character
		EnemyState = EEnemyState::EES_Attacking;
		UE_LOG(LogTemp, Warning, TEXT("Attack Player"));
		// TODO: Attack montage
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		GetWorldTimerManager().SetTimer(
			PatrolTimer,
			this,
			&AEnemy::PatrolTimerFinished,
			PatrolDelayTime
		);
	}
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (!EnemyController || !Target) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(20.f);

	FNavPathSharedPtr NavPath;
	EnemyController->MoveTo(MoveRequest, &NavPath);
	TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
	for (auto& Point : PathPoints)
	{
		const FVector& Location = Point.Location;
		DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 5.f);
	}
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetIndex = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetIndex];
	}

	return nullptr;
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if (EnemyState == EEnemyState::EES_Chasing) return;

	if (SeenPawn->ActorHasTag(FName("SlashCharacter")))
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		CombatTarget = SeenPawn;
		UE_LOG(LogTemp, Warning, TEXT("Set the combat target"));

		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(SeenPawn);
			UE_LOG(LogTemp, Warning, TEXT("Seen Pawn, now Chasing!"));
		}
	}
}

/**************************************************************************************/

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (!Target) return false;

	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	DRAW_DEBUG_SPHERE_SingleFrame(GetActorLocation());
	DRAW_DEBUG_SPHERE_SingleFrame(Target->GetActorLocation());
	return DistanceToTarget <= Radius;
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

/**************************************************************************************/

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}

	if (Attributes && Attributes->IsAlive())
	{
		DirectionHitReact(ImpactPoint);
	}
	else 
	{ 
		Die(); 
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}

	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);

	}
}

void AEnemy::DirectionHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = this->GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - this->GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0) { Theta *= -1.f; }

	FName Section("FromBack");

	if (Theta >= -45.f && Theta <= 45.f) { Section = FName("FromFront"); }
	else if (Theta >= -135.f && Theta < -45.f) { Section = FName("FromLeft"); }
	else if (Theta >= 45.f && Theta < 135.f) { Section = FName("FromRight"); }

	PlayHitReactMontage(Section);

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));

	//DRAW_DEBUG_ARROW(GetActorLocation(), GetActorLocation() + Forward * 60.f, FColor::Red);
	//DRAW_DEBUG_ARROW(GetActorLocation(), GetActorLocation() + ToHit * 60.f, FColor::Green);
	//DRAW_DEBUG_ARROW(GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, FColor::Blue);
}

float AEnemy::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
	CombatTarget = EventInstigator->GetPawn();
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	MoveToTarget(CombatTarget);

	return DamageAmount;
}

