// Fill out your copyright notice in the Description page of Project Settings.

#include "DownTheVerse.h"
#include "Missile.h"


// Sets default values
AMissile::AMissile() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> MissileMesh;
		FConstructorStatics()
			: MissileMesh(TEXT("/Game/Models/missile"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissileMesh0"));
	MissileMesh->SetStaticMesh(ConstructorStatics.MissileMesh.Get());
	RootComponent = MissileMesh;

	TurnSpeed = 50.f;
	CurrentForwardSpeed = 500.f;
	MissileDamage = 25.f;
	CurrentTarget = NULL;
}

// Called when the game starts or when spawned
void AMissile::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AMissile::Tick( float DeltaTime ) {
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.f, 0.f);

	// Move plan forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);

	if (CurrentTarget) {
		FQuat TargetDirection((CurrentTarget->GetActorLocation() - this->GetActorLocation()).Rotation());
		FQuat CurrentDirection(GetActorRotation());
		FQuat NewDirection = FQuat::Slerp(CurrentDirection, TargetDirection, .01f);
		SetActorRotation((NewDirection).Rotator());
	}

	// Call any parent class Tick implementation
	Super::Tick( DeltaTime );
}

void AMissile::ReceiveHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) {
	Super::ReceiveHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if (Other) {
		Other->TakeDamage(MissileDamage, FDamageEvent(), this->GetInstigatorController(), this);
	}
	this->Destroy();
}

void AMissile::SetCurrentTarget(AActor* NewTarget) { CurrentTarget = NewTarget; }

void AMissile::SetCurrentSpeed(float NewSpeed) { CurrentForwardSpeed = NewSpeed; }
void AMissile::SetCurrentDamage(float NewDamage) { MissileDamage = NewDamage; }