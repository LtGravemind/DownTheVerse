// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "CombatHUD.generated.h"

/**
 * 
 */
UCLASS()
class DOWNTHEVERSE_API ACombatHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ACombatHUD(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY(Category = Default, EditAnywhere, BlueprintReadWrite)
	AActor* CurrentTarget;
};
