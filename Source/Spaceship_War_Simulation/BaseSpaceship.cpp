// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSpaceship.h"
#include "GreenLaser.h"
#include "RedLaser.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseSpaceship::ABaseSpaceship()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseSpaceship::BeginPlay()
{
	Super::BeginPlay();
	
	cone = GetWorld()->SpawnActor<AConeCollision>(AConeCollision::StaticClass(), GetActorLocation(), GetActorRotation());
}

void ABaseSpaceship::CalculateAcceleration(FVector TargetVel, float ForceMultiplier)
{
    FVector curVel = velocity;

    // set to length
    float m = TargetVel.Size();
    TargetVel.X *= targetSpeed / m;
    TargetVel.Y *= targetSpeed / m;
    TargetVel.Z *= targetSpeed / m;

    FVector goalSpeedForce = TargetVel - curVel;
    goalSpeedForce = goalSpeedForce.GetClampedToMaxSize(maxForce * DEFAULT_FORCE_MULTIPLIER * ForceMultiplier);

    acceleration = acceleration + goalSpeedForce;
}

void ABaseSpaceship::Move(float DeltaTime)
{
    //Update Position 
    FVector oldPos = GetActorLocation();
    FVector newPos = GetActorLocation() + velocity * DeltaTime;
    SetActorLocation(newPos);

    //Update Rotation
    FRotator rot = UKismetMathLibrary::MakeRotFromY(newPos - oldPos);
    SetActorRotation(rot);
}

void ABaseSpaceship::UpdateVelocity(float DeltaTime)
{
    //Update Velocity
    FVector newVel = velocity + acceleration * DeltaTime;
    velocity = newVel;

    //Max Speed
    if (velocity.Size() > maxSpeed) {
        velocity.Normalize();
        velocity = velocity * maxSpeed;
    }
}

void ABaseSpaceship::CheckBounds(float DeltaTime)
{
    FVector c = GetActorLocation();
    if (c.X <= xMin || c.X >= xMax || c.Y <= yMin || c.Y >= yMax || c.Z <= zMin || c.Z >= zMax) {
        acceleration = FVector(0.0f);
        if (OutOfBoundVelocity == nullptr) {
            FVector v = velocity;
            if (c.X <= xMin || c.X >= xMax) {
                if (c.X <= xMin) {
                    if (v.X < 0) {
                        v.X *= -1.0f;
                    }
                }
                else if (c.X >= xMax) {
                    if (v.X >= 0) {
                        v.X *= -1.0f;
                    }
                }
            }
            if (c.Y <= yMin || c.Y >= yMax) {
                if (c.Y <= yMin) {
                    if (v.Y < 0) {
                        v.Y *= -1.0f;
                    }
                }
                else if (c.Y >= yMax) {
                    if (v.Y >= 0) {
                        v.Y *= -1.0f;
                    }
                }
            }
            if (c.Z <= zMin || c.Z >= zMax) {
                if (c.Z < zMin) {
                    if (v.Z < 0) {
                        v.Z *= -1.0f;
                    }
                }
                else if (c.Z >= zMax) {
                    if (v.Z >= 0) {
                        v.Z *= -1.0f;
                    }
                }
            }
            OutOfBoundVelocity = new FVector(v.X, v.Y, v.Z);
        }
        else {
            if (OutOfBoundTime >= 5) {
                SetActorLocation(FVector(FMath::RandRange(xMin, xMax), FMath::RandRange(yMin, yMax), FMath::RandRange(zMin, zMax)));
                OutOfBoundTime = 0;
            }
            else {
                OutOfBoundTime += DeltaTime;
            }
        }
        CalculateAcceleration(*OutOfBoundVelocity, BOUND_FORCE_MULTIPLIER);
    }
    else {
        if (OutOfBoundVelocity != nullptr) {
            OutOfBoundVelocity = nullptr;
        }
    }
}

FVector ABaseSpaceship::PredictFuturePosition(ABaseSpaceship* OtherEntity)
{   
    FVector predictedPos = OtherEntity->GetActorLocation() + OtherEntity->velocity * PREDICT_TIME_INTO_FUTURE * FMath::RandRange(0.8, 1.0);
    
    return predictedPos;
}

void ABaseSpaceship::FollowTarget(ABaseSpaceship* TargetSpaceship)
{
    // Let the taget spaceship know that they are being followed
    TargetSpaceship->beingFollowed = true;

    // Calculate targetSpaceship next position when the laser arrive
    FVector TargetSpaceshipFuturePos = PredictFuturePosition(TargetSpaceship);

    // Set velocity to there which is to Follow TargetSpaceship
    FVector dir = TargetSpaceshipFuturePos - GetActorLocation();
    dir.Normalize();
    CalculateAcceleration(dir, FOLLOW_FORCE_MULTIPLIER);
}

// Called every frame
void ABaseSpaceship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    acceleration = FVector(0.0f);

    if (HP <= 0) {
        destroyCountDown += DeltaTime;

        if (ExplosionSystem && ExplosionSystem->Template)
        {
            ExplosionSystem->ToggleActive();
        }

        if (Spaceship && destroyCountDown >= 0.1)
        {
            Spaceship->SetVisibility(false);
        }

        if (destroyCountDown >= 5.0) {
            cone->Destroy();
            Destroy();
        }
    }
    else {
        cone->SetActorLocation(GetActorLocation() + (GetTransform().GetUnitAxis(EAxis::Y) * 20000.0));
        FRotator newConeRot = GetActorRotation();
        newConeRot.Roll -= 90;
        cone->SetActorRotation(newConeRot);

        // If being followed, give up on following other and try to dodge
        if (beingFollowed) {
            if (lastChangeVel > NormalTimeToChangeVelocity) {
                // try to speed up and set random velocity to dodge
                newRandomVelocity = FVector(FMath::RandRange(-1.0, 1.0), FMath::RandRange(-1.0, 1.0), FMath::RandRange(-1.0, 1.0));
                newRandomVelocity.Normalize();
                NormalTimeToChangeVelocity = FMath::RandRange(1.0, 3.0);
                lastChangeVel = 0;
            }
            else {
                lastChangeVel += DeltaTime;
            }
            CalculateAcceleration(newRandomVelocity, DOGE_FORCE_MULTIPLIER);
        }
        else if (cone->TargetSpaceship == NULL) {
            if (lastChangeVel > NormalTimeToChangeVelocity) {
                newRandomVelocity = FVector(FMath::RandRange(-1.0, 1.0), FMath::RandRange(-1.0, 1.0), FMath::RandRange(-0.1, 0.1));
                newRandomVelocity.Normalize();
                NormalTimeToChangeVelocity = FMath::RandRange(2.0, 5.0);
                lastChangeVel = 0;
            }
            else {
                lastChangeVel += DeltaTime;
            }
            CalculateAcceleration(newRandomVelocity, NORMAL_FORCE_MULTIPLIER / 2);
        }
    }
}

void ABaseSpaceship::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->GetClass()->IsChildOf(AGreenLaser::StaticClass()) || OtherActor->GetClass()->IsChildOf(ARedLaser::StaticClass())) {
        if (!Indestructible) {
            HP -= 5;
        }
    }
    else if (OtherActor->GetClass()->IsChildOf(ABaseSpaceship::StaticClass())) {
        if (!Indestructible) {
            HP = 0;
        }
    }
}

