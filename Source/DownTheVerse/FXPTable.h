// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "FXPTable.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct FXPTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** Full Path of Blueprint */
	UPROPERTY(BlueprintReadWrite, Category = "XPTable", EditAnywhere)
	int32 RequiredXP;
};