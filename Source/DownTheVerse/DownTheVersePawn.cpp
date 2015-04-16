// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "DownTheVerse.h"
#include "DownTheVersePawn.h"
#include "Missile.h"
#include "EFighter.h"
#include "CombatHUD.h"
#include "CombatGameState.h"

ADownTheVersePawn::ADownTheVersePawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) {
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Models/UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	RootComponent = PlaneMesh;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->TargetArmLength = 240.0f; // The camera follows at this distance behind the character Default: 240
	SpringArm->SocketOffset = FVector(0.f,0.f,60.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;

	DefaultArmRotation = SpringArm->RelativeRotation;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller
	Camera->FieldOfView = 90.f;

	// Set handling parameters
	Acceleration = 500.f;
	PitchSpeed = 50.f;
	RollSpeed = 125.f;
	MaxSpeed = 4000.f;
	MinSpeed = 500.f;
	CurrentForwardSpeed = 500.f;
	MaxHealth = 100.f;
	CurrentHealth = 100.f;
}

void ADownTheVersePawn::Tick(float DeltaSeconds) {
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);

	// Move plan forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);

	// Calculate change in rotation this frame
	FRotator DeltaRotation(0,0,0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

	// Rotate plane
	AddActorLocalRotation(DeltaRotation);
	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);
}

void ADownTheVersePawn::ReceiveHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) {
	Super::ReceiveHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Set velocity to zero upon collision
	CurrentForwardSpeed = 0.f;
	this->TakeDamage(10.f, FDamageEvent(), NULL, Other);

	FVector temp = HitLocation - Other->GetActorLocation();
	SetActorRotation(temp.Rotation());
}

float ADownTheVersePawn::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.f) {
		CurrentHealth = 0.f;
		SetLifeSpan(0.001f);
	}
	if (CurrentHealth > MaxHealth) {
		CurrentHealth = MaxHealth;
	}
	return 0.f;
}

void ADownTheVersePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);

	// Bind our control axis' to callback functions
	InputComponent->BindAxis("Thrust", this, &ADownTheVersePawn::ThrustInput);
	InputComponent->BindAxis("MoveUp", this, &ADownTheVersePawn::MoveUpInput);
	InputComponent->BindAxis("MoveRight", this, &ADownTheVersePawn::MoveRightInput);
	InputComponent->BindAction("NextTarget", IE_Pressed, this, &ADownTheVersePawn::IncrementTarget);
	InputComponent->BindAction("PreviousTarget", IE_Pressed, this, &ADownTheVersePawn::DecrementTarget);
}

void ADownTheVersePawn::ThrustInput(float Val)
{
	// Is there no input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = 0.f;
	if (bHasInput) { CurrentAcc = Val * Acceleration; }
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void ADownTheVersePawn::MoveUpInput(float Val)
{
	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * PitchSpeed * -1.f);

	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f); //Remember to remove

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ADownTheVersePawn::MoveRightInput(float Val)
{
	// Target yaw speed is based on input
	float TargetRollSpeed = (Val * RollSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ADownTheVersePawn::IncrementTarget() {
	ACombatGameState* State = Cast<ACombatGameState>(GetWorld()->GetGameState());
	ACombatHUD* HUD = Cast<ACombatHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (State->TargetList.Num() == 0) {
		return;
	}
	bool bIsInList = State->TargetList.Contains(HUD->CurrentTarget);
	if (HUD->CurrentTarget != NULL && bIsInList) {
		int Index = State->TargetList.Find(HUD->CurrentTarget);
		Index++;
		if (State->TargetList.Num() <= Index) {
			HUD->CurrentTarget = State->TargetList[0];
		}
		else {
			HUD->CurrentTarget = State->TargetList[Index];
		}
	}
	else {
		HUD->CurrentTarget = State->TargetList[0];
	}
}

void ADownTheVersePawn::DecrementTarget() {
	ACombatGameState* State = Cast<ACombatGameState>(GetWorld()->GetGameState());
	ACombatHUD* HUD = Cast<ACombatHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	auto TargetList = State->TargetList;
	if (TargetList.Num() == 0) {
		return;
	}
	bool bIsInList = TargetList.Contains(HUD->CurrentTarget);
	if (bIsInList) {
		int Index = TargetList.Find(HUD->CurrentTarget);
		Index--;
		if (Index < 0) {
			HUD->CurrentTarget = TargetList[TargetList.Num() - 1];
		}
		else {
			HUD->CurrentTarget = TargetList[Index];
		}
	}
	else {
		HUD->CurrentTarget = TargetList[0];
	}
}