// Fill out your copyright notice in the Description page of Project Settings.


#include "UE4FPS/Public/QuestBase.h"
#include "UE4FPS/Public/ObjectiveBase.h"

QuestBase::QuestBase()
{
	Objectives = TArray<ObjectiveBase>();
}

QuestBase::~QuestBase()
{
}

void QuestBase::CreateQuestLine(int QId, FString QName, class ObjectiveBase QObjective)
{
	Id = QId;
	Name = QName;
	Objective = QObjective;

	Objectives.Add(Objective);
}
