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
	MaxHealth = 25.f;
	CurrentHealth = 25.f;
	AvoidDamage = FVector::ZeroVector;
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

	// Move drone forward
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.f, 0.f);
	AddActorLocalOffset(LocalMove, true);

	if (!CurrentTarget) {
		Super::Tick(DeltaTime);
		return;
	}

	FCollisionQueryParams TraceParams(true);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult hit(ForceInit);

	FVector Origin, BoxExtents;
	GetActorBounds(false, Origin, BoxExtents);
	FVector Avoid = GetActorLocation() + GetActorRotation().Vector() * CurrentForwardSpeed * SightMultiplier;
	GetWorld()->SweepSingle(hit,
							this->GetActorLocation(),
							Avoid,
							FQuat::Identity,
							FCollisionShape::MakeBox(BoxExtents),
							TraceParams,
							FCollisionObjectQueryParams());

	if (hit.GetActor() && hit.GetActor() == CurrentTarget) {
		if (Time > TimePerShot) {
			Time = 0.f;
			FVector EndTrace = GetActorLocation() + GetActorRotation().Vector() * 10000;

			hit = FHitResult(ForceInit);
			GetWorld()->LineTraceSingle(hit, GetActorLocation(), EndTrace, TraceParams, FCollisionObjectQueryParams());
			DrawDebugLine(GetWorld(), GetActorLocation(), EndTrace, FColor::Red, false, 1, 0, 12.333);
		}
	}
	else if (hit.GetActor() && hit.GetActor() != CurrentTarget && Cast<AEFighter>(hit.GetActor()) == nullptr) {
		FVector AvoidanceForce = hit.Location - hit.GetActor()->GetActorLocation();
		AvoidanceForce += AvoidDamage;
		AvoidDamage = FVector::ZeroVector;
		this->RotateTowards(AvoidanceForce + this->GetActorLocation());
	}
	else {
		FVector Direction = this->FlockInfluence(CurrentTarget->GetActorLocation() - this->GetActorLocation());
		Direction += AvoidDamage;
		AvoidDamage = FVector::ZeroVector;
		this->RotateTowards(Direction);
	}
	Super::Tick( DeltaTime );
}

void AEFighter::RotateTowards(FVector RotationDirection) {
	FQuat TargetDirection(RotationDirection.Rotation());
	FQuat CurrentDirection(GetActorRotation());
	FQuat NewDirection = FQuat::Slerp(CurrentDirection, TargetDirection, 0.01f);
	SetActorRotation((NewDirection).Rotator());
}

FVector AEFighter::FlockInfluence(FVector RotationDirection) {
	FVector Alignment, Cohesion, Seperation;
	FCollisionQueryParams SphereParams(FName(TEXT("Boid")), false, this);
	TArray<FOverlapResult> Overlaps;
	if (GetWorld()->OverlapMulti(Overlaps,
								 GetActorLocation(),
								 FQuat::Identity,
								 FCollisionShape::MakeSphere(800.f),
								 SphereParams,
								 FCollisionObjectQueryParams())) {
		//DrawDebugSphere(GetWorld(), GetActorLocation(), 800.f, 32, FColor::Red);
		int32 NumberOfFighters = 1;
		for (int32 Idx = 0; Idx < Overlaps.Num(); ++Idx) {
			FOverlapResult const& Overlap = Overlaps[Idx];
			AEFighter* Mine = Cast<AEFighter>(Overlap.GetActor());
			// If this actor is valid, do something with it  
			if (Mine != nullptr) {
				NumberOfFighters++;
				FVector DistanceBase = GetActorLocation() - Mine->GetActorLocation();
				float DistanceNormal = DistanceBase.Size();

				if (DistanceNormal < 800.f) {
					Alignment += Mine->GetActorRotation().Vector();
				}
				if (DistanceNormal < 640.f) {
					Cohesion += Mine->GetActorLocation();
				}
				if (DistanceNormal < 512.f) {
					Seperation += DistanceBase / (DistanceNormal * DistanceNormal);
				}
			}
		}
		if (NumberOfFighters > 1) {
			Alignment /= NumberOfFighters - 1;
			Cohesion /= NumberOfFighters - 1;
			Cohesion -= GetActorLocation();
		}
		Alignment.Normalize();
		Cohesion.Normalize();
		Seperation.Normalize();
		RotationDirection.Normalize();
		return (90.f*RotationDirection + 2.5f*Alignment + 2.5f*Cohesion + 2.5f*Seperation);
	}
	else {
		return RotationDirection;
	}
}

void AEFighter::ReceiveHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) {
	Super::ReceiveHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	this->Destroy();
}

float AEFighter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.f) {
		CurrentHealth = 0.f;
		SetLifeSpan(0.001f);
	}
	if (CurrentHealth > MaxHealth) {
		CurrentHealth = MaxHealth;
	}
	AvoidDamage = GetActorRotation().Vector() * -1 * 10000;
	return 0.f;
}

void AEFighter::Destroyed() {
	Super::Destroyed();
}

void AEFighter::ToggleTargeted() {
	bIsFighterTargeted = bIsFighterTargeted ? false : true;
}