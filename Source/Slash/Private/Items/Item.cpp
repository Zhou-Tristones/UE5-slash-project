
#include "Items/Item.h"

#include "Components/SphereComponent.h"
#include "Characters/SlashCharacter.h"
#include "NiagaraComponent.h"

#include "Slash/DebugMacros.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(ItemMesh);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	EmbersEffect->SetupAttachment(GetRootComponent());

}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();

	//DRAW_DEBUG_SPHERE(StartLocation);
	//DRAW_DEBUG_BOX(StartLocation, FVector(25.f));
	//DRAW_DEBUG_VECTOR(StartLocation, EndLocation);
	//DRAW_DEBUG_ARROW(StartLocation, EndLocation);

	/*int32 AvgInt = Avg<int32>(1, 3);
	UE_LOG(LogTemp, Warning, TEXT("Avg of 1 and 3: %d"), AvgInt);

	UE_LOG(LogTemp, Warning, TEXT("Begin Play called!"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, FString("Item OnScreen Message"));
	}*/


	//将回调函数AItem::OnSphereOverlap绑定到OnComponentBeginOverlap委托上
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);

	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

}

float AItem::TransformedSin(float Value)
{
	return Amplitude * FMath::Sin(Value * TimeConstant);
}

float AItem::TransformedCos(float Value)
{
	return Amplitude * FMath::Cos(Value * TimeConstant);
}

void AItem::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//const FString OtherActorName = OtherActor->GetName();
	//if (GEngine) GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, OtherActorName);

	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter) SlashCharacter->SetOverlappingItem(this);
}


void AItem::OnSphereEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	//const FString OtherActorName = FString("Ending Overlap with: ") + OtherActor->GetName();
	//if (GEngine) GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Cyan, OtherActorName);

	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter) SlashCharacter->SetOverlappingItem(nullptr);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin(RunningTime)));
		AddActorWorldRotation(FRotator(0.f, 5.f, 0.f));
	}

}

