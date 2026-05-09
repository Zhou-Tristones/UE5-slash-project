// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemTypes.h"
#include "Item.generated.h"

class USphereComponent;


UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/** 正弦运动的振幅 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sine Parameters");
	float Amplitude = 1.f;

	/** 正弦运动的时间系数 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sine Parameters");
	float TimeConstant = 5.f;

	/***
	* 使物品进行正弦运动
	* @param Value 可以是运行时间
	* @return 浮点数
	***/
	UFUNCTION(BlueprintPure)
	float TransformedSin(float Value);

	/***
	* 使物品进行余弦运动
	* @param Value 可以是运行时间
	* @return 浮点数
	***/
	UFUNCTION(BlueprintPure)
	float TransformedCos(float Value);

protected:
	/** 物品的网格体组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	EItemState ItemState = EItemState::EIS_Hovering;

	// 取平均值
	template<typename T>
	T Avg(T First, T Second);

protected:
	/**
	* 触发重叠事件的回调函数
	* @param OverlappedComponent
	* @param OtherActor 与物品重叠的Actor
	*/
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/***
	* 触发结束重叠事件的回调函数
	* @param OverlappedComponent
	* @param OtherActor 与物品结束重叠的Actor
	***/
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** 球形碰撞组件 */
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* EmbersEffect;


private:
	/** 游戏的运行时间 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime;

};

template<typename T>
inline T AItem::Avg(T First, T Second) { return (First + Second) / 2; }

