// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UE4FPSCharacter.h"
#include "UE4FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UE4FPS/Public/QuestBase.h"
#include "UE4FPS/Public/ObjectiveBase.h"
#include "UE4FPS/Public/AICharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "UE4FPS/UE4FPSHUD.h"
#include "MotionControllerComponent.h"
#include "Engine.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AUE4FPSCharacter

AUE4FPSCharacter::AUE4FPSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// MashineGun Static Mesh
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MGMesh(TEXT("StaticMesh'/Game/Weapons/MachineGun/Meshes/MachineGun.MachineGun'"));
	MachineGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MachineGun"));
	//MachineGun->SetStaticMesh(MGMesh.Object);
	if (MachineGunMesh)
	{
		MachineGun->SetStaticMesh(MachineGunMesh);
	}
	MachineGun->SetOnlyOwnerSee(true);
	MachineGun->bCastDynamicShadow = false;
	MachineGun->CastShadow = false;
	MachineGun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	MachineGun->SetupAttachment(RootComponent);
	MachineGun->ComponentTags.Add("Gun");

	FP_MuzzleLocation = CreateDefaultSubobject<USphereComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(MachineGun);
	
	FP_MuzzleLocation->SetSphereRadius(.1f);
	FP_MuzzleLocation->bVisible = true;
	FP_MuzzleLocation->bHiddenInGame = false;

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	Tags.Add("Player");

	//static ConstructorHelpers::FObjectFinder<UParticleSystem> Explosion(TEXT("ParticleSystem'/Game/FirstPerson/ParticleSystem/P_Explosion.P_Explosion'"));
	//HitEffect = Explosion.Object;

	InitialHealth = 100.f;
	InitialDamage = 10.f;
	InitialRange = 2500.f;
	InitialShootingSpeed = 1.f;

	LootboxInv = 0;

	AllQuests = TArray<QuestBase*>();
	ActiveQuests = TArray<QuestBase*>();
	CompletedQuests = TArray<QuestBase*>();

	//static ConstructorHelpers::FObjectFinder<UClass> PBP(TEXT("Class'/Game/FirstPerson/FPWeapon/BP_LaserBullet.BP_LaserBullet_C'"));
	//Projectile = PBP.Object;
}

void AUE4FPSCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	CurrentHealth = InitialHealth;
	CurrentDamage = InitialDamage;
	CurrentMoney = 0.f;
	CurrentRange = InitialRange;
	CurrentShootingSpeed = InitialShootingSpeed;
	PrevShootingSpeed = InitialShootingSpeed;

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	MachineGun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("GripPoint"));
	MachineGun->SetRelativeScale3D(FVector(10.f, 10.f, 10.f));
	MachineGun->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	MachineGun->SetRelativeLocation(FVector(2.5f, 15.f, -10.f));

	FP_MuzzleLocation->AttachToComponent(MachineGun, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	FP_MuzzleLocation->SetRelativeLocation(FVector(.0f, -3.f, 1.7f));

	Mesh1P->SetHiddenInGame(false, true);

	FireDown = false;
	CanShoot = true;

	SetupQuests();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUE4FPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUE4FPSCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AUE4FPSCharacter::OnFireStop);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AUE4FPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUE4FPSCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUE4FPSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUE4FPSCharacter::LookUpAtRate);
}

void AUE4FPSCharacter::OnFire()
{
	FireDown = true;
	if (CanShoot)
	{
		GetWorld()->GetTimerManager().SetTimer(ShootHandle, this, &AUE4FPSCharacter::Fire, CurrentShootingSpeed, true, 0.f);
	}
}

void AUE4FPSCharacter::OnFireStop()
{
	FireDown = false;
}

void AUE4FPSCharacter::Fire()
{
	if (FireDown)
	{
		CanShoot = false;

		FHitResult* HitResult = new FHitResult();
		FVector StartTrace = FP_MuzzleLocation->GetComponentLocation();
		FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
		FVector EndTrace = ((ForwardVector * CurrentRange) + StartTrace);
		FCollisionQueryParams * TraceParams = new FCollisionQueryParams();
		TraceParams->AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
		{
			//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, true);
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("You hit: %s"), *HitResult->Actor->GetName()));

			if (HitResult->GetActor())
			{
				if (HitEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult->Location, HitResult->GetActor()->GetActorRotation());
				}

				FVector Loc = FP_MuzzleLocation->GetComponentLocation();
				FRotator ProjectileRot = UKismetMathLibrary::FindLookAtRotation(Loc, HitResult->Location);
				FActorSpawnParameters SpawnInfo;

				if (Projectile)
				{
					GetWorld()->SpawnActor<AActor>(Projectile, Loc, ProjectileRot, SpawnInfo);
				}

				if (HitResult->GetActor()->ActorHasTag("Enemy"))
				{
					AAICharacter* AIEnemy = Cast<AAICharacter>(HitResult->GetActor());
					AIEnemy->UpdateHealth(-CurrentDamage);
				}
			}
		}
		else
		{
			FVector Loc = FP_MuzzleLocation->GetComponentLocation();
			FRotator CameraRot = FirstPersonCameraComponent->GetComponentRotation();
			FActorSpawnParameters SpawnInfo;
			GetWorld()->SpawnActor<AActor>(Projectile, Loc, CameraRot, SpawnInfo);
		}

		// try and play the sound if specified
		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation != NULL)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}

	if (!FireDown)
	{
		GetWorld()->GetTimerManager().ClearTimer(ShootHandle);
		CanShoot = true;

		if (FireDown)
		{
			GetWorld()->GetTimerManager().SetTimer(ShootHandle, this, &AUE4FPSCharacter::Fire, CurrentShootingSpeed, true, 0.f);
		}
	}

	if (FireDown && PrevShootingSpeed != CurrentShootingSpeed)
	{
		GetWorld()->GetTimerManager().ClearTimer(ShootHandle);
		CanShoot = true;
		GetWorld()->GetTimerManager().SetTimer(ShootHandle, this, &AUE4FPSCharacter::Fire, CurrentShootingSpeed, true, 0.f);
		PrevShootingSpeed = CurrentShootingSpeed;
	}

}

void AUE4FPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AUE4FPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AUE4FPSCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUE4FPSCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUE4FPSCharacter::UpdateHealth(float Damage)
{
	CurrentHealth += Damage;
}

void AUE4FPSCharacter::UpdateMoney(float Money)
{
	CurrentMoney += Money;
	if (Money > 0)
	{
		TotalMoneyCollected += Money;
	}
}

void AUE4FPSCharacter::UpdateDamage(float Damage)
{
	CurrentDamage += Damage;
}

void AUE4FPSCharacter::UpdateMaxHealth(float NewHealth)
{
	InitialHealth += NewHealth;
	CurrentHealth = InitialHealth;
}

void AUE4FPSCharacter::UpdateShootingRange(float Range)
{
	CurrentRange += Range;
}

void AUE4FPSCharacter::UpdateShootingSpeed(float Speed)
{
	PrevShootingSpeed = CurrentShootingSpeed;
	CurrentShootingSpeed += Speed;
}

void AUE4FPSCharacter::UpdatePlayername(FName Playername)
{
	PlayerName = PlayerName;
}

void AUE4FPSCharacter::SetupQuests()
{
	Objective = new ObjectiveBase();
	Objective->CreateObjective(1, "FirstMoney", "Collect 1$");
	Quest = new QuestBase();
	Quest->CreateQuestLine(1, "Time to get some money", *Objective);
	AllQuests.Add(Quest);

	Objective = new ObjectiveBase();
	Objective->CreateObjective(2, "FirstKill", "Kill one enemy");
	Quest = new QuestBase();
	Quest->CreateQuestLine(2, "One must be hunted", *Objective);
	AllQuests.Add(Quest);

	Objective = new ObjectiveBase();
	Objective->CreateObjective(3, "FirstDLC", "Buy one DLC");
	Quest = new QuestBase();
	Quest->CreateQuestLine(3, "Here goes your money", *Objective);
	AllQuests.Add(Quest);

	Objective = new ObjectiveBase();
	Objective->CreateObjective(4, "FirstLootbox", "Open one Lootbox");
	Quest = new QuestBase();
	Quest->CreateQuestLine(4, "Loot, Loot, Loot", *Objective);
	AllQuests.Add(Quest);

	ActiveQuests.Add(AllQuests[0]);
	AllQuests.RemoveAt(0);

	CurrentQuestName = ActiveQuests[0]->Name;
	CurrentQuestObjective = ActiveQuests[0]->Objectives[0].Description;

	CheckObjective();
}

void AUE4FPSCharacter::CheckObjective()
{
	if (ActiveQuests[0]->Objectives[0].Name == "FirstMoney")
	{
		MoneyObjectiveGoal = CurrentMoney + 1.f;
	}
	else if (ActiveQuests[0]->Objectives[0].Name == "FirstKill")
	{
		KillObjectiveGoal = KilledEnemies + 1;
	}
	else if (ActiveQuests[0]->Objectives[0].Name == "FirstDLC")
	{
		DLCObjectiveGoal = BoughtDLCs + 1;
	}
	else if (ActiveQuests[0]->Objectives[0].Name == "FirstLootbox")
	{
		LootboxObjectiveGoal = OpenedLootboxes + 1;
	}
}

void AUE4FPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ActiveQuests.IsValidIndex(0))
	{
		if (ActiveQuests[0]->Objectives[0].Name == "FirstMoney")
		{
			if (CurrentMoney >= MoneyObjectiveGoal)
			{
				//GEngine->AddOnScreenDebugMessage(69, 15.f, FColor::Red, "FirstMoney Quest completed!", true);
				UpdateQuest();
			}
		}
		else if (ActiveQuests[0]->Objectives[0].Name == "FirstKill")
		{
			if (KilledEnemies >= KillObjectiveGoal)
			{
				//GEngine->AddOnScreenDebugMessage(70, 15.f, FColor::Red, "FirstKill Quest completed!", true);
				UpdateQuest();
			}
		}
		else if (ActiveQuests[0]->Objectives[0].Name == "FirstDLC")
		{
			if (BoughtDLCs >= DLCObjectiveGoal)
			{
				//GEngine->AddOnScreenDebugMessage(71, 15.f, FColor::Red, "FirstDLC Quest completed!", true);
				UpdateQuest();
			}
		}
		else if (ActiveQuests[0]->Objectives[0].Name == "FirstLootbox")
		{
			if (OpenedLootboxes >= LootboxObjectiveGoal)
			{
				//GEngine->AddOnScreenDebugMessage(72, 15.f, FColor::Red, "FirstLootbox Quest completed!", true);
				UpdateQuest();
			}
		}
	}
}

void AUE4FPSCharacter::UpdateQuest()
{
	//GEngine->AddOnScreenDebugMessage(100, 15.f, FColor::Red, "Removed active quest " + ActiveQuests[0]->Name, true);
	CompletedQuests.Add(ActiveQuests[0]);
	ActiveQuests.RemoveAt(0);

	if (AllQuests.IsValidIndex(0))
	{
		ActiveQuests.Add(AllQuests[0]);
		AllQuests.RemoveAt(0);
		//GEngine->AddOnScreenDebugMessage(102, 15.f, FColor::Red, "New active quest is " + ActiveQuests[0]->Name, true);
		if (ActiveQuests.IsValidIndex(0))
		{
			CurrentQuestName = ActiveQuests[0]->Name;
			//GEngine->AddOnScreenDebugMessage(102, 15.f, FColor::Red, "Changed displayed quest name to " + CurrentQuestName, true);
			CurrentQuestObjective = ActiveQuests[0]->Objectives[0].Description;
			//GEngine->AddOnScreenDebugMessage(102, 15.f, FColor::Red, "Changed displayed quest objective to " + CurrentQuestObjective, true);

			//GEngine->AddOnScreenDebugMessage(104, 15.f, FColor::Red, "Checking objective goal", true);
			CheckObjective();
		}
	}
	else
	{
		CurrentQuestName = "";
		CurrentQuestObjective = "";
	}
}