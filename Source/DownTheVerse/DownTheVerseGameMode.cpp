// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "DownTheVerse.h"
#include "DownTheVerseGameMode.h"
#include "DownTheVersePawn.h"

ADownTheVerseGameMode::ADownTheVerseGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = ADownTheVersePawn::StaticClass();
}
