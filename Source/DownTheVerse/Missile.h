// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Missile.generated.h"

UCLASS()
class DOWNTHEVERSE_API AMissile : public AActor
{
	GENERATED_BODY()
	
	/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MissileMesh;

public:	
	// Sets default values for this actor's properties
	AMissile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called when actor collides with primative
	virtual void ReceiveHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);

private:
	// Current Target
	AActor* CurrentTarget;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Plane, EditAnywhere)
	float TurnSpeed;

	/** Current forward speed */
	float CurrentForwardSpeed;

	float MissileDamage;

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetCurrentTarget(AActor* NewTarget);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetCurrentSpeed(float NewSpeed);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SetCurrentDamage(float NewDamage);
};
