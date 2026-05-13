
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ItemTypes.h"
#include "Interfaces/HitInterface.h"

#include "BaseCharacter.generated.h"

class AWeapons;
class UAttributeComponent;
class UAnimMontage;

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;

	/** Components */
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	/** Character Action */
	virtual void Attack();
	
	virtual bool CanAttack();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	virtual void Die();

	bool IsAlive();

	/** Play Montage */
	void PlayMontageSection(UAnimMontage* MontageToPlay, const FName& SectionName);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	virtual void PlayHitReactMontage(const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* MontageToPlay, const TArray<FName>& SectionNames);

	/** Character get hit */
	void DirectionHitReact(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);

	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);

	/** 角色装备的武器 */
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapons* EquippedWeapon;

	/** Animation Montages */
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	/** Montage Sections Manager */
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> DeathMontageSections;

	void DisableCapsule();

private:
	/** Combat effects */
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	UParticleSystem* HitParticles;

};
