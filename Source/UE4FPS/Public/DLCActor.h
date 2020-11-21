// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DLCActor.generated.h"

UCLASS()
class UE4FPS_API ADLCActor : public AActor
{
	GENERATED_BODY()
	
	/** Box collider for actor.*/
	UPROPERTY(VisibleDefaultsOnly, Category = "Collider")
	class UBoxComponent* Collider;

public:	
	// Sets default values for this actor's properties
	ADLCActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Begin component overlap
	UFUNCTION(BlueprintCallable, Category = "Collider")
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
