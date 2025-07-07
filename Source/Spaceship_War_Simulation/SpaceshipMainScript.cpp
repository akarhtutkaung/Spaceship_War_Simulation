// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceshipMainScript.h"
#include "SpaceshipX.h"
#include "SpaceshipY.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpaceshipMainScript::ASpaceshipMainScript()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpaceshipMainScript::BeginPlay()
{
	Super::BeginPlay();

	CreateSpaceships();

	Spaceships[0]->Indestructible = true;
}

void ASpaceshipMainScript::CreateSpaceships()
{
	// Initialize SpaceshipA actors
	for (int i = 0; i < totalSpaceshipX; i++) {
		float x = FMath::RandRange(xMin, xMax);
		float y = FMath::RandRange(yMin, yMax);
		float z = FMath::RandRange(zMin, zMax);
		FVector* loc = new FVector(x, y, z);
		// Here check if the position is in colliding with another entity
		ABaseSpaceship* actor = GetWorld()->SpawnActor<ASpaceshipX>(ASpaceshipX::StaticClass(), *loc, GetActorRotation());
		actor->targetSpeed = defaultSpeed;
		actor->maxSpeed = defaultMaxSpeed;

		float velX, velY, velZ;
		do {
			velX = FMath::RandRange(-1.0, 1.0);
		} while (!(velX >= -0.1 && velX <= 0.1));
		do {
			velY = FMath::RandRange(-1.0, 1.0);
		} while (!(velY >= -0.1 && velY <= 0.1));
		do {
			velZ = FMath::RandRange(-0.1, 0.1);
		} while (!(velZ >= -0.01 && velZ <= 0.01));

		FVector newVel = FVector(velX, velY, velZ);
		newVel.Normalize();
		newVel *= actor->targetSpeed;
		actor->velocity = newVel;
		actor->xMax = xMax;
		actor->xMin = xMin;
		actor->yMax = yMax;
		actor->yMin = yMin;
		actor->zMax = zMax;
		actor->zMin = zMin;
		actor->maxForce = maxForce;
		Spaceships.Add(actor);
	}

	// Initialize SpaceshipB actors
	for (int i = 0; i < totalSpaceshipY; i++) {
		float x = FMath::RandRange(xMin, xMax);
		float y = FMath::RandRange(yMin, yMax);
		float z = FMath::RandRange(zMin, zMax);
		FVector* loc = new FVector(x, y, z);
		// Here check if the position is in colliding with another entity
		ABaseSpaceship* actor = GetWorld()->SpawnActor<ASpaceshipY>(ASpaceshipY::StaticClass(), *loc, GetActorRotation());

		actor->targetSpeed = defaultSpeed;
		actor->maxSpeed = defaultMaxSpeed;

		float velX, velY, velZ;
		do {
			velX = FMath::RandRange(-1.0, 1.0);
		} while (!(velX >= -0.1 && velX <= 0.1));
		do {
			velY = FMath::RandRange(-1.0, 1.0);
		} while (!(velY >= -0.1 && velY <= 0.1));
		do {
			velZ = FMath::RandRange(-0.1, 0.1);
		} while (!(velZ >= -0.01 && velZ <= 0.01));

		FVector newVel = FVector(velX, velY, velZ);
		newVel.Normalize();
		newVel *= actor->targetSpeed;
		actor->velocity = newVel;
		actor->xMax = xMax;
		actor->xMin = xMin;
		actor->yMax = yMax;
		actor->yMin = yMin;
		actor->zMax = zMax;
		actor->zMin = zMin;
		actor->maxForce = maxForce;
		Spaceships.Add(actor);
	}
}

// Called every frame
void ASpaceshipMainScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (OurPlayerController)
	{
			//Cut instantly to camera one.
			OurPlayerController->SetViewTarget(Spaceships[0]);
	}
}

