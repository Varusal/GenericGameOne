// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UE4FPS_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	/** Behavior Tree component reference.*/
	class UBehaviorTreeComponent* BehaviorComp;

	/** Blackboard component reference.*/
	class UBlackboardComponent* BlackboardComp;

public:
	/** Constructor*/
	AMyAIController();

	/** Blackboard key*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName BlackboardKey = "Target";

	/** Blackboard key*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName BlackboardWaitingKey = "WaitingTime";

	/** Executes right when the AIController possess a Pawn.*/
	virtual void OnPossess(APawn* Pawn) override;

	/** Sets the sensed target in the blackboard.*/
	void SetSeenTarget(APawn* Pawn);

	void SetWaitingTime(float Time);
};
