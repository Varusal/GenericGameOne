// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UE4FPSCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AUE4FPSCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** MachineGun mesh: 1st person view (see only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MachineGun;

	/** MachineGun mesh: Editor selectable*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* MachineGunMesh;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USphereComponent* FP_MuzzleLocation;

	/** First person camera. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Bullet hit particle effect. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* HitEffect;

	/** Projectile. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> Projectile;

public:
	AUE4FPSCharacter();

protected:
	virtual void BeginPlay();

public:

	/** Amount of Lootboxes to buy and open to complete the objective. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest Objective")
	int LootboxObjectiveGoal;

	/** Amount of DLCs to buy to complete the objective. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest Objective")
	int DLCObjectiveGoal;

	/** Amount of enemies to kill to complete the objective. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest Objective")
	int KillObjectiveGoal;

	/** Amount of money to collect to complete the objective. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest Objective")
	float MoneyObjectiveGoal;

	/** Check current quest objective. */
	UFUNCTION(BlueprintCallable, Category = "Quest Objective")
	void CheckObjective();

	/** Update current quest. */
	UFUNCTION(BlueprintCallable, Category = "Update Quest")
	void UpdateQuest();

	/** Initial setup for questlines. */
	UFUNCTION(BlueprintCallable, Category = "Setup Quest")
	void SetupQuests();

	/** All quests. */
	class TArray<class QuestBase*> AllQuests;

	/** Active quests. */
	class TArray<class QuestBase*> ActiveQuests;

	/** Completed quests. */
	class TArray<class QuestBase*> CompletedQuests;

	/** New quest. */
	class QuestBase* Quest;

	/** New objective. */
	class ObjectiveBase* Objective;

	/** Current quest name. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	FString CurrentQuestName;

	/** Current quest objective. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	FString CurrentQuestObjective;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** Initial player health. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float InitialHealth;

	/** Current player health. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentHealth;

	/** Initial player damage. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float InitialDamage;

	/** Current player damage. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentDamage;

	/** Initial player shooting range. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float InitialRange;

	/** Initial player shooting range. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentRange;

	/** Initial player shooting speed. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float InitialShootingSpeed;

	/** Current player shooting speed. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentShootingSpeed;

	/** Previous player shooting speed. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float PrevShootingSpeed;

	/** Current player money. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentMoney;

	/** Total amount of opened Lootboxes. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	int OpenedLootboxes;

	/** Total amount of bought DLCs. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	int BoughtDLCs;

	/** Total amounmt of killed enemies. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int KilledEnemies;

	/** Total amount of gained money. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float TotalMoneyCollected;

	/** Playername. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Playername")
	FName PlayerName;

	/** State if the player can shoot. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Check States")
	bool CanShoot;

	/** State if the fire key is held down. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Check States")
	bool FireDown;

	/** Timer for shooting (if player held fire key down). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shoot timer")
	FTimerHandle ShootHandle;

	/** Holds the amount of lootboxes the player has. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lootbox Inventory")
	int LootboxInv;

	/** Update Health with taken damage. */
	UFUNCTION(BlueprintCallable, Category = "Update Stats")
	void UpdateHealth(float Damage);

	/** Update Money. */
	UFUNCTION(BlueprintCallable, Category = "Update Stats")
	void UpdateMoney(float Money);

	/** Update Damage.*/
	UFUNCTION(BlueprintCallable, Category = "Update Stats")
	void UpdateDamage(float Damage);

	/** Update Max-Health.*/
	UFUNCTION(BlueprintCallable, Category = "Update Stats")
	void UpdateMaxHealth(float Damage);

	/** Update Shooting-Range. */
	UFUNCTION(BlueprintCallable, Category = "Update Stats")
	void UpdateShootingRange(float Range);

	/** Update Shooting-Speed. */
	UFUNCTION(BlueprintCallable, Category = "Update Stats")
	void UpdateShootingSpeed(float Speed);

	/** Update Playername. */
	UFUNCTION(BlueprintCallable, Category = "Update Playername")
	void UpdatePlayername(FName Name);

	/** Fire Method*/
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void Fire();

	/** Tick - Called every frame. */
	virtual void Tick(float DeltaTime) override;

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Stops shooting. */
	void OnFireStop();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

