// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSpaceship.h"
#include "SpaceshipMainScript.generated.h"

UCLASS()
class Spaceship_War_Simulation_API ASpaceshipMainScript : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpaceshipMainScript();

private:
	UPROPERTY()
		TArray<ABaseSpaceship*> Spaceships;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CreateSpaceships();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		float xMax = 50400;

	UPROPERTY(EditAnywhere)
		float xMin = -100800;

	UPROPERTY(EditAnywhere)
		float yMax = 50400;

	UPROPERTY(EditAnywhere)
		float yMin = -75600;

	UPROPERTY(EditAnywhere)
		float zMax = 60700.0;

	UPROPERTY(EditAnywhere)
		float zMin = 34700.0;

	UPROPERTY(EditAnywhere)
		int totalSpaceshipX = 20;

	UPROPERTY(EditAnywhere)
		int totalSpaceshipY = 20;

	UPROPERTY(EditANywhere)
		float maxForce = 100;

	UPROPERTY(EditAnywhere)
		float defaultSpeed = 10000;

	UPROPERTY(EditAnywhere)
		float defaultMaxSpeed = 20000;

	UPROPERTY(EditAnywhere)
		float seperation = 20;
};
