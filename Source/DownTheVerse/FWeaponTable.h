// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "FWeaponTable.generated.h"

/**
*
*/
USTRUCT(Blueprintable)
struct FWeaponTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** Full Path of Blueprint */
	UPROPERTY(BlueprintReadWrite, Category = "WeaponTable", EditAnywhere)
	float LaserRange;

	UPROPERTY(BlueprintReadWrite, Category = "WeaponTable", EditAnywhere)
	float LaserDamage;

	UPROPERTY(BlueprintReadWrite, Category = "WeaponTable", EditAnywhere)
	float MissileSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "WeaponTable", EditAnywhere)
	float MissileDamage;
};