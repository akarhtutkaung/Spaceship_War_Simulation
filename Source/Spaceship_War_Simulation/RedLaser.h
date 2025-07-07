// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RedLaser.generated.h"

UCLASS()
class Spaceship_War_Simulation_API ARedLaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARedLaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float speed = 500000;
	FVector direction;
	FVector velocity;
	bool allDataSet = false;
	float existingTime = 0;
	const float timeToLive = 3;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetDirection(FVector dir);
	void SetSpeed(float s);
	void SetVelocity(FVector v);
	void AllDataSet(bool b);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
