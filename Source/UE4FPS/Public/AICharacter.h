// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

UCLASS()
class UE4FPS_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnSeesPlayer(APawn* Pawn);

public:
	// Sets default values for this character's properties
	AAICharacter();

	/** The component which is used for the 'seeing' sense of AI.*/
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

	/** The Behavior Tree of the AICharacter*/
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	/** Static Mesh that is child of the main skeletal mesh.*/
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* StaticMesh;

	/** Initial health of the AI.*/
	UPROPERTY(BlueprintReadWrite ,EditAnywhere, Category = "Stats")
	float InitialHealth;

	/** Drop chance for Lootboxes.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
	float LootboxDropChance;

	/** Current health of the AI.*/
	UPROPERTY(EditAnywhere, Category = "Stats")
	float CurrentHealth;

	/** Current damage that will be dealed by the AI.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
	float CurrentDamage;

	/** Shoot speed of the AI.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
	float CurrentShootSpeed;

	/** Min drop of money from the AI.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
	float MoneyMinDrop;

	/** Max drop of money from the AI.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
	float MoneyMaxDrop;

	/** Explode effect.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Exploding Particle Effect")
	class UParticleSystem* Explode;

	/** Update current health of the AI.*/
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateHealth(float Damage);

	/** Sets current health to initial health.*/
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetHealth();

	/** Sets Player Pawn and Waiting time.*/
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetKeys();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
