// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "UE4FPS/Public/MyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "UE4FPS/UE4FPSCharacter.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initializing the pawn sensing component
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingCompoent"));

	//Set the peripheral vision angle to 90 degrees
	PawnSensingComp->SetPeripheralVisionAngle(90.f);

	UActorComponent* skMesh = GetComponentByClass(USkeletalMeshComponent::StaticClass());
	if (skMesh != nullptr)
	{
		skMesh->DestroyComponent();
		skMesh->SetActive(false);
	}

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	GetCapsuleComponent()->SetCapsuleHalfHeight(100.f);
	GetCapsuleComponent()->SetCapsuleRadius(100.f);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StaticMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	StaticMesh->SetRelativeScale3D(FVector(10.f, 10.f, 10.f));
	StaticMesh->SetRelativeLocation(FVector(140.f, -850.f, -20.f));

	InitialHealth = 100.f;
	CurrentHealth = InitialHealth;

	CurrentDamage = 5.f;
	CurrentShootSpeed = 1.f;

	MoneyMinDrop = .50f;
	MoneyMaxDrop = 1.50f;

	LootboxDropChance = 80;

	//static ConstructorHelpers::FObjectFinder<UParticleSystem> Explosion(TEXT("ParticleSystem'/Game/Enemies/ParticleSystem/Par_Meteora_Explode_01.Par_Meteora_Explode_01'"));
	//Explode = Explosion.Object;

	Tags.Add("Enemy");
}

void AAICharacter::UpdateHealth(float Damage)
{
	CurrentHealth += Damage;
	if (CurrentHealth <= 0)
	{
		AUE4FPSCharacter* Player = Cast<AUE4FPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		Player->UpdateMoney(FMath::FRandRange(MoneyMinDrop, MoneyMaxDrop));
		int LootboxChance = FMath::FRandRange(0, 100);
		if (LootboxChance >= LootboxDropChance)
		{
			Player->LootboxInv += 1;
		}
		Player->KilledEnemies += 1;
		if (Explode)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explode, GetActorLocation(), GetActorRotation(), FVector(.25F));
		}
		Destroy();
	}
}

void AAICharacter::SetHealth()
{
	CurrentHealth = InitialHealth;
}

void AAICharacter::SetKeys()
{
	AMyAIController* AIController = Cast<AMyAIController>(GetController());
	if (AIController)
	{
		AIController->SetSeenTarget(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		AIController->SetWaitingTime(CurrentShootSpeed);
	}
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Register the function that is going to fire when the AI sees a Pawn
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AAICharacter::OnSeesPlayer);
	}
}

void AAICharacter::OnSeesPlayer(APawn* Pawn)
{
	AMyAIController* AIController = Cast<AMyAIController>(GetController());

	//Set the seen target on the blackboard
	if (AIController)
	{
		//AIController->SetSeenTarget(Pawn);
		//AIController->SetWaitingTime(CurrentShootSpeed);
	}
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

