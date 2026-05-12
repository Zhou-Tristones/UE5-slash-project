
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

	/**
	* 播放蒙太奇
	*/
	virtual void PlayAttackMontage();
	virtual void PlayHitReactMontage(const FName& SectionName);
	void DirectionHitReact(const FVector& ImpactPoint);

	virtual bool CanAttack();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();


	// Combat effects
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	UParticleSystem* HitParticles;

	/**
	* Components
	*/

	/** 角色装备的武器 */
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapons* EquippedWeapon;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	/**
	* 动画蒙太奇
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

public:	

};
