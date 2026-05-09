
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	// Current Health
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health = 100;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth = 100;

public:
	void ReceiveDamage(float Damage);
	float GetHealthPercent();
	bool IsAlive();

		
};
