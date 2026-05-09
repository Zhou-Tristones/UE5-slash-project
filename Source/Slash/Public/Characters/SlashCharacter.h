
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class AWeapons;

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EKeyPressed();
	void ThreeKeyPressed();

	/***
	* 角色主要动作
	***/
	void Attack();

	/***
	* 播放蒙太奇
	***/
	void PlayAttackMontage();
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool CanAttack();

	void PlayEquipMontage(FName SectionName);
	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();


private:
	// 角色状态
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	/** 摄像机组件 */
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	/** 角色毛发组件 */
	UPROPERTY(VisibleAnywhere, Category = "Hair")
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = "Hair")
	UGroomComponent* Eyebrows;

	/** 与角色重叠的物品 */
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	/** 角色装备的武器 */
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapons* EquippedWeapon;

	/**
	* 动画蒙太奇
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;

public:
	// 设置与角色重叠的物品
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	// 获取角色状态
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

};
