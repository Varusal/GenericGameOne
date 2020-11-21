// Fill out your copyright notice in the Description page of Project Settings.


#include "LootboxActor.h"
#include "Components/BoxComponent.h"
#include "UE4FPS/UE4FPSCharacter.h"

// Sets default values
ALootboxActor::ALootboxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collider->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ALootboxActor::OnOverlapBegin);
	Collider->bHiddenInGame = false;
	RootComponent = Collider;

	this->SetActorHiddenInGame(false);
}

// Called when the game starts or when spawned
void ALootboxActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALootboxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALootboxActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		AUE4FPSCharacter* PlayerCharacter = Cast<AUE4FPSCharacter>(OtherActor);
		PlayerCharacter->OpenedLootboxes += 1;
	}
}

