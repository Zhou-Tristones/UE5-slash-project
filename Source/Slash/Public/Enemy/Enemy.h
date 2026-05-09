// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UHealthBarComponent;
class UAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	void DirectionHitReact(const FVector& ImpactPoint);

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

protected:
	virtual void BeginPlay() override;

	/**
	* Play montage functions
	*/
	void PlayHitReactMontage(const FName& SectionName);
	void Die();


	bool InTargetRange(AActor* Target, double Radius);

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
private:

	/**
	* Components
	*/
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* EnemySensing;

	/**
	* Animation Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	UParticleSystem* HitParticles;

	/**
	* Combat Param
	*/
	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	/**
	* Navigation
	*/
	UPROPERTY()
	class AAIController* EnemyController;

	// 当前的巡逻目标
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	// 巡逻目标数组
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PartolTime;
	void PatrolTimerFinished();


};
