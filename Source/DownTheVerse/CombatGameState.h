// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "CombatGameState.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class DOWNTHEVERSE_API ACombatGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ACombatGameState(const FObjectInitializer& ObjectInitializer);
private:
public:
	UPROPERTY(Category = Default, EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> TargetList;
};
