// Fill out your copyright notice in the Description page of Project Settings.

#include "DownTheVerse.h"
#include "CombatHUD.h"

ACombatHUD::ACombatHUD(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer) {
	CurrentTarget = NULL;
}