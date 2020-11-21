// Fill out your copyright notice in the Description page of Project Settings.


#include "Lootbox.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "UE4FPSCharacter.h"

ULootbox::ULootbox()
{
	ResetLoot();
}

void ULootbox::OpenLootbox(bool PlayerHasKey)
{
	if (PlayerHasKey)
	{
		CalculateLoot();
		AdjustStats();
	}
}

void ULootbox::CalculateLoot()
{
	HealthUpgrade = FMath::RandBool();
	DamageUpgrade = FMath::RandBool();
	SpeedUpgrade = FMath::RandBool();
	RangeUpgrade = FMath::RandBool();
	MoneyUpgrade = FMath::RandBool();
}

void ULootbox::AdjustStats()
{
	AUE4FPSCharacter* Player = Cast<AUE4FPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (HealthUpgrade)
	{
		Player->UpdateHealth(FMath::FRandRange(1.f, 15.f));
	}
	if (DamageUpgrade)
	{
		Player->UpdateDamage(FMath::FRandRange(1.f, 15.f));
	}
	if (SpeedUpgrade && Player->CurrentShootingSpeed > 0.1f)
	{
		Player->UpdateShootingSpeed(-0.02f);
	}
	if (RangeUpgrade)
	{
		Player->UpdateShootingRange(FMath::FRandRange(10.f, 150.f));
	}
	if (MoneyUpgrade)
	{
		Player->UpdateMoney(FMath::FRandRange(1.f, 150.f));
	}
}

void ULootbox::ResetLoot()
{
	HealthUpgrade = false;
	DamageUpgrade = false;
	SpeedUpgrade = false;
	RangeUpgrade = false;
	MoneyUpgrade = false;
}
