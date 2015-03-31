#pragma once

#include "GameFramework/Pawn.h"
#include "EFighter.generated.h"

UCLASS(config = Game)
class DOWNTHEVERSE_API AEFighter : public APawn
{
	GENERATED_BODY()

	/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* DroneSight;

public:
	// Sets default values for this pawn's properties
	AEFighter(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Recieved hit
	virtual void ReceiveHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	// Take Damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);

	// Destroyed
	virtual void Destroyed();

private:
	// Current Target
	APawn* CurrentTarget;

	/** How quickly forward speed changes */
	UPROPERTY(Category = Plane, EditAnywhere)
		float Acceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Plane, EditAnywhere)
		float PitchSpeed;

	UPROPERTY(Category = Plane, EditAnywhere)
		float RollSpeed;

	/** Max forward speed */
	UPROPERTY(Category = Speed, EditAnywhere)
		float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category = Speed, EditAnywhere)
		float MinSpeed;

	UPROPERTY(Category = Sight, EditAnywhere)
		float SightMultiplier;

	/** sphere component */
	UPROPERTY(VisibleAnywhere, Category = "Switch Components")
	class UCapsuleComponent* TargetArea;

	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

	//Time elapsed since last shot
	float Time;

	//Time in between shots
	UPROPERTY(Category = Weapon, EditAnywhere)
	float TimePerShot;

	UPROPERTY(Category = Health, EditAnywhere)
		float CurrentHealth;

	UPROPERTY(Category = Health, EditAnywhere)
		float MaxHealth;
	
public:
	/** Whether the Fighter is currently targeted */
	bool bIsFighterTargeted;

	/** Toggle whether Fighter is currently targeted */
	void ToggleTargeted();
};
