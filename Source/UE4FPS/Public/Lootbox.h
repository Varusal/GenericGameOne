// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Lootbox.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, BlueprintType, Blueprintable)
class UE4FPS_API ULootbox : public UObject
{
	GENERATED_BODY()
	
public:
	/** Constructor. */
	ULootbox();

	/** Should upgrade health. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Lootbox upgrade checks")
	bool HealthUpgrade;

	/** Should upgrade damage. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Lootbox upgrade checks")
	bool DamageUpgrade;

	/** Should upgrade shoot speed. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Lootbox upgrade checks")
	bool SpeedUpgrade;

	/** Should upgrade shoot range. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Lootbox upgrade checks")
	bool RangeUpgrade;

	/** Should upgrade money. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Lootbox upgrade checks")
	bool MoneyUpgrade;

	/** Open Lootbox. */
	UFUNCTION(BlueprintCallable, Category = "Open Lootbox")
	void OpenLootbox(bool PlayerHasKey);

	/** Open Lootbox. */
	UFUNCTION(BlueprintCallable, Category = "Calculate Loot")
	void CalculateLoot();

	/** Adjust Player Stats. */
	UFUNCTION(BlueprintCallable, Category = "Adjust Player Stats")
	void AdjustStats();

	/** Reset after open lootbox. */
	UFUNCTION(BlueprintCallable, Category = "Reset Loot")
	void ResetLoot();
};
