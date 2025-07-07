// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConeCollision.h"
#include <limits>
#include "Particles/ParticleSystemComponent.h"
#include "BaseSpaceship.generated.h"

UCLASS()
class Spaceship_War_Simulation_API ABaseSpaceship : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSpaceship();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void CreateLaser() {};
	void CalculateAcceleration(FVector TargetVel, float ForceMultiplier = 1);
	void Move(float DeltaTime);
	void UpdateVelocity(float DeltaTime);
	void CheckBounds(float DeltaTime);
	FVector PredictFuturePosition(ABaseSpaceship* OtherEntity);
	void FollowTarget(ABaseSpaceship* TargetSpaceship);

protected:
	UParticleSystemComponent* ExplosionSystem;
	UStaticMeshComponent* Spaceship;

protected:
	float prevLaserTime = 0;
	float lastChangeVel = std::numeric_limits<double>::infinity();
	float eachVelChangeInterval = 0.8f;
	float NormalTimeToChangeVelocity;
	AConeCollision* cone;
	FVector* OutOfBoundVelocity = nullptr;
	float OutOfBoundTime = 0;
	FVector newRandomVelocity;
	float destroyCountDown = 0;

protected:
	const float LASER_SPEED_MULTIPLIER = 1000.0f;
	const float DEFAULT_FORCE_MULTIPLIER = 10.0f;
	const float BOUND_FORCE_MULTIPLIER = 40.0f;
	const float FOLLOW_FORCE_MULTIPLIER = 10.0f;
	const float DOGE_FORCE_MULTIPLIER = 20.0f;
	const float NORMAL_FORCE_MULTIPLIER = 5.0f;
	const float PREDICT_TIME_INTO_FUTURE = 2.0f;
	const float LASER_SHOT_INTERVAL = 0.02f;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Variables
public:
	UPROPERTY(EditAnywhere)
		bool Indestructible = false;

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
		float HP = 100;

	UPROPERTY(EditAnywhere)
		FVector velocity;

	UPROPERTY(EditAnywhere)
		FVector acceleration;

	UPROPERTY(EditAnywhere)
		float maxSpeed = 1000.0f;

	UPROPERTY(EditAnywhere)
		float targetSpeed = 100.0f;

	UPROPERTY(EditAnywhere)
		float maxForce = 10.0f;

	UPROPERTY(EditAnywhere)
		float laserSpeed = 50.0f;

	UPROPERTY(EditAnywhere)
		float xMax = 50400;

	UPROPERTY(EditAnywhere)
		float xMin = -100800;

	UPROPERTY(EditAnywhere)
		float yMax = 50400;

	UPROPERTY(EditAnywhere)
		float yMin = -75600;

	UPROPERTY(EditAnywhere)
		float zMax = 40700.0;

	UPROPERTY(EditAnywhere)
		float zMin = 24700.0;

	UPROPERTY(EditAnywhere)
		bool beingFollowed = false;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
