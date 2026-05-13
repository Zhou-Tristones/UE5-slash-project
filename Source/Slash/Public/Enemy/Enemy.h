// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	AEnemy();

	/** <AActor> */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;
	virtual void Destroyed() override;
	/** </AActor> */

	/** <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	/** </IHitInterface> */

protected:

	/** <AActor> */
	virtual void BeginPlay() override;
	/** </AActor> */

	/** <BaseCharacter> */
	virtual void Die() override;
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;
	/** </BaseCharacter> */

	/** Enemy State */
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose;
	
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:

	/** AI Behavior */
	void InitializeEnemy();

	/** Patrol Behavior*/
	void CheckPatrolTarget();
	AActor* ChoosePatrolTarget();
	void StartPatrolling();
	void PatrolTimerFinished();
	void ClearPatrolTimer();

	/** Combat Behavior */
	void CheckCombatTarget();
	void StartAttackTimer();
	void ClearAttackTimer();
	void LoseInterest();
	void HideHealthBar();
	void ShowHealthBar();
	void SpawnDefaultWeapon();

	/** Navigation */
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPawnSensingComponent
	void MoveToTarget(AActor* Target);
	void ChaseTarget();

	/** State Queries */
	bool InTargetRange(AActor* Target, double Radius);
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();

	/** General Components */
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* EnemySensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapons> WeaponClass;

	UPROPERTY()
	class AAIController* EnemyController;

	/** Combat Variables*/ 
	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 800.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed = 300.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 8.f;

	/** Patrol Variables */
	// Current Patrol Target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	// Patrol Target Array
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float PatrollingSpeed = 125.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation | Time")
	float PatrolDelayTime = 5.f;

};
