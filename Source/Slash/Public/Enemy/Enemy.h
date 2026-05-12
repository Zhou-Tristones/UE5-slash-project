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

	
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;


	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	/**
	* Play montage functions
	*/
	virtual void Die() override;


	/**
	* Enemy State
	*/
	bool InTargetRange(AActor* Target, double Radius);

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
	
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	/**
	* Enemy Movement
	*/
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	void CheckCombatTarget();
	void CheckPatrolTarget();

	/**
	* Enemy Sense
	*/
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

private:

	/**
	* Components
	*/

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* EnemySensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapons> WeaponClass;

	/**
	* Combat
	*/


	// Combat params
	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 800.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	/**
	* Navigation
	*/
	UPROPERTY()
	class AAIController* EnemyController;

	// Current Patrol Target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	// Patrol Target Array
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation | Time")
	float PatrolDelayTime = 5.f;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();


};
