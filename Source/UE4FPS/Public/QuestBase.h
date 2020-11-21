// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE4FPS/Public/ObjectiveBase.h"

/**
 * 
 */
class UE4FPS_API QuestBase
{

public:
	QuestBase();
	~QuestBase();

public:
	int Id;
	FString Name;
	class ObjectiveBase Objective;
	class TArray<class ObjectiveBase> Objectives;
	class TArray<class ObjectiveBase> CompletedObjectives;
	void CreateQuestLine(int QId, FString QName, class ObjectiveBase QObjective);
};
