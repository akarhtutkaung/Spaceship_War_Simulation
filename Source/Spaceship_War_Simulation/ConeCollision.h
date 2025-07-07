// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConeCollision.generated.h"

UCLASS()
class Spaceship_War_Simulation_API AConeCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConeCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	AActor* GetNearestEnemySpaceship();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UStruct* EnemyBaseClass;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
		AActor* MySpaceship;

	UPROPERTY(VisibleAnywhere)
		AActor* TargetSpaceship;

	UPROPERTY(VisibleAnywhere)
		TArray<AActor*> SpaceshipsInsideFOV;
};
