
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

	/** <AActor> */
	virtual void Tick(float DeltaTime) override;
	/** </AActor> */


protected:
	/** <AActor> */
	virtual void BeginPlay() override;
	/** </AActor> */

	/** Character Combat */
	virtual void Attack();
	virtual bool CanAttack();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	void DirectionHitReact(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);

	virtual void Die();
	bool IsAlive();
	void DisableCapsule();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	/** Play Montage */
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	virtual void PlayHitReactMontage(const FName& SectionName);

	/** Components */
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapons* EquippedWeapon;

	/** Montage Sections Manager */
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> DeathMontageSections;


private:

	void PlayMontageSection(UAnimMontage* MontageToPlay, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* MontageToPlay, const TArray<FName>& SectionNames);

	/** Combat effects */
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	UParticleSystem* HitParticles;

	/** Animation Montages */
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;


};
