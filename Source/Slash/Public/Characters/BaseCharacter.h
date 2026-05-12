
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

	virtual void Attack();
	
	virtual void Die();

	/** Play Montage */
	virtual void PlayAttackMontage();
	virtual void PlayHitReactMontage(const FName& SectionName);
	void DirectionHitReact(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);

	virtual bool CanAttack();
	bool IsAlive();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();



	/**
	* Components
	*/

	/** 角色装备的武器 */
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapons* EquippedWeapon;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	/** Animation Montages */
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

private:
	/** Combat effects */
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	UParticleSystem* HitParticles;

};
