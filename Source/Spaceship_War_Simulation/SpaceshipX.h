// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpaceship.h"
#include "SpaceshipX.generated.h"

/**
 * 
 */
UCLASS()
class Spaceship_War_Simulation_API ASpaceshipX : public ABaseSpaceship
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASpaceshipX();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void CreateLaser();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
