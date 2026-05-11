#pragma once

/** 角色装备状态枚举 */
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	/** 未装备武器 */
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	/** 装备单手武器 */
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	/** 装备双手武器 */
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

/** 角色动作状态枚举 */
UENUM(BlueprintType)
enum class EActionState : uint8
{
	/** 无状态 */
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	/** 正在攻击 */
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	/** 正在装备武器 */
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Death5 UMETA(DisplayName = "Death5"),
	EDP_Death6 UMETA(DisplayName = "Death6")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking")
};
