// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AMyAIController::AMyAIController()
{
	// Initialize the behavior tree and blackboard components.
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AMyAIController::OnPossess(APawn* Pawn)
{
	Super::OnPossess(Pawn);

	//Get the possessed Pawn. If it's the AAICharacter.
	// Initialize it's blackboard and start it's corresponding behavior tree.
	AAICharacter* AICharacter = Cast<AAICharacter>(Pawn);
	if (AICharacter)
	{
		if (AICharacter->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
			BehaviorComp->StartTree(*AICharacter->BehaviorTree);
		}
	}
}

void AMyAIController::SetSeenTarget(APawn* Pawn)
{
	//Register the pawn that the AI has seen in the blackboard
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(BlackboardKey, Pawn);
	}
}

void AMyAIController::SetWaitingTime(float Time)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsFloat(BlackboardWaitingKey, Time);
	}
}
