// Fill out your copyright notice in the Description page of Project Settings.


#include "UE4FPS/Public/ObjectiveBase.h"

ObjectiveBase::ObjectiveBase()
{
}

ObjectiveBase::~ObjectiveBase()
{
}

void ObjectiveBase::CreateObjective(int OId, FString OName, FString ODescription)
{
	Id = OId;
	Name = OName;
	Description = ODescription;
}


