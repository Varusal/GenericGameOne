// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UE4FPS_API ObjectiveBase
{
public:
	ObjectiveBase();
	~ObjectiveBase();

public:
	int Id;
	FString Name;
	FString Description;
	void CreateObjective(int OId, FString OName, FString ODescription);
};
