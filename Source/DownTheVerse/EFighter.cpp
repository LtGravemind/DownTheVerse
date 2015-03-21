// Fill out your copyright notice in the Description page of Project Settings.

#include "DownTheVerse.h"
#include "EFighter.h"


// Sets default values
AEFighter::AEFighter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set handling parameters
	Acceleration = 500.f;
	PitchSpeed = 50.f;
	RollSpeed = 125.f;
	MaxSpeed = 4000.f;
	MinSpeed = 500.f;
	CurrentForwardSpeed = 500.f;
	bIsFighterTargeted = false;
	CurrentTarget = NULL;
	SightMultiplier = 3.f;
	Time = 0.f;
	TimePerShot = 5.f;
}

// Called when the game starts or when spawned
void AEFighter::BeginPlay() {
	Super::BeginPlay();
	auto PC = GetWorld()->GetFirstPlayerController();
	if (PC) {
		if (PC->GetPawn()) {
			CurrentTarget = PC->GetPawn();
		}
	}
}

// Called every frame
void AEFighter::Tick( float DeltaTime ) {
	Time += DeltaTime;
	if (CurrentTarget) {
		const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.f, 0.f);

		// Move plan forwards (with sweep so we stop when we collide with things)
		AddActorLocalOffset(LocalMove, true);

		FCollisionQueryParams TraceParams(true);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;
		TraceParams.AddIgnoredActor(this);

		//const FName TraceTag("BoxTag");
		//GetWorld()->DebugDrawTraceTag = TraceTag;
		//TraceParams.TraceTag = TraceTag;

		FHitResult hit(ForceInit);

		FVector Origin, BoxExtents;
		GetActorBounds(false, Origin, BoxExtents);
		FVector Avoid = GetActorLocation() + GetActorRotation().Vector() * CurrentForwardSpeed * SightMultiplier;
		GetWorld()->SweepSingle(hit, this->GetActorLocation(), Avoid, FQuat::Identity, FCollisionShape::MakeBox(BoxExtents), TraceParams, FCollisionObjectQueryParams());
		if (hit.GetActor() && hit.GetActor() != CurrentTarget) {
			FVector AvoidanceForce = hit.Location - hit.GetActor()->GetActorLocation();
			FQuat TargetDirection((AvoidanceForce + this->GetActorLocation()).Rotation());
			FQuat CurrentDirection(GetActorRotation());
			FQuat NewDirection = FQuat::Slerp(CurrentDirection, TargetDirection, .01f);
			SetActorRotation((NewDirection).Rotator());
		}
		hit = FHitResult(ForceInit);
		FVector Attack = GetActorLocation() + GetActorRotation().Vector() * CurrentForwardSpeed * SightMultiplier * 2;
		GetWorld()->SweepSingle(hit, this->GetActorLocation(), Attack, FQuat::Identity, FCollisionShape::MakeBox(BoxExtents), TraceParams, FCollisionObjectQueryParams());
		if (hit.GetActor() == CurrentTarget) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::SanitizeFloat(Time));
			if (Time > TimePerShot) {
				Time = 0.f;
				FVector EndTrace = GetActorLocation() + GetActorRotation().Vector() * 10000;

				hit = FHitResult(ForceInit);
				GetWorld()->LineTraceSingle(hit, GetActorLocation(), EndTrace, TraceParams, FCollisionObjectQueryParams());
				DrawDebugLine(GetWorld(), GetActorLocation(), EndTrace, FColor::Red, false, 1, 0, 12.333);
			}
		}
		else if ((CurrentTarget->GetActorLocation() - GetActorLocation()).Size() > CurrentForwardSpeed) {
			FQuat TargetDirection((CurrentTarget->GetActorLocation() - this->GetActorLocation()).Rotation());
			FQuat CurrentDirection(GetActorRotation());
			FQuat NewDirection = FQuat::Slerp(CurrentDirection, TargetDirection, .01f);
			SetActorRotation((NewDirection).Rotator());
		}
	}
	Super::Tick( DeltaTime );
}

void AEFighter::ReceiveHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) {
	Super::ReceiveHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

void AEFighter::Destroyed() {
	Super::Destroyed();
}

void AEFighter::ToggleTargeted() {
	bIsFighterTargeted = bIsFighterTargeted ? false : true;
}