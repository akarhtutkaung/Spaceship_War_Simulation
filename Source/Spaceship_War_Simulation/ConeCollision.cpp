// Fill out your copyright notice in the Description page of Project Settings.


#include "ConeCollision.h"
#include "SpaceshipX.h"
#include "SpaceshipY.h"
#include <limits>

// Sets default values
AConeCollision::AConeCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    // Create Cone shape collision to check if other are infront
    UStaticMeshComponent* FrontCollisionCone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontCollisionCone"));   
    FrontCollisionCone->SetupAttachment(RootComponent);
    FrontCollisionCone->SetCollisionProfileName("Trigger");
    FrontCollisionCone->OnComponentBeginOverlap.AddUniqueDynamic(this, &AConeCollision::OnOverlapBegin);
    FrontCollisionCone->OnComponentEndOverlap.AddUniqueDynamic(this, &AConeCollision::OnOverlapEnd);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeVisualAsset(TEXT("StaticMesh'/Game/Assets/Shape/Shape_Cone.Shape_Cone'"));
    if (ConeVisualAsset.Succeeded())
    {
        FrontCollisionCone->SetStaticMesh(ConeVisualAsset.Object);
        FrontCollisionCone->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
        FrontCollisionCone->SetWorldScale3D(FVector(300.0f, 300.0f, 200.0f));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Cone Collision Visual Asset Load Failed"));
    }
}

// Called when the game starts or when spawned
void AConeCollision::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConeCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (TargetSpaceship == NULL) {
        TargetSpaceship = GetNearestEnemySpaceship();
    }
}

AActor* AConeCollision::GetNearestEnemySpaceship()
{
    if (SpaceshipsInsideFOV.Num() > 0) {
        float minDist = std::numeric_limits<float>::infinity();
        AActor* nearestEnemySpaceship = NULL;
        for (AActor* a : SpaceshipsInsideFOV) {
            float dist = FVector::Dist(MySpaceship->GetActorLocation(), a->GetActorLocation());
            if (dist < minDist) {
                minDist = dist;
                nearestEnemySpaceship = a;
            }
        }
        return nearestEnemySpaceship;
    }
    return NULL;
}

void AConeCollision::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (EnemyBaseClass) {
        if (OtherActor->GetClass()->IsChildOf(EnemyBaseClass)) {
            //GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "Overlap Begin Function Called");
            SpaceshipsInsideFOV.Add(OtherActor);
        }
    }
}

void AConeCollision::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (EnemyBaseClass) {
        if (OtherActor->GetClass()->IsChildOf(EnemyBaseClass)) {
            if (OtherActor == TargetSpaceship) {
                //GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "Overlap End Function Called");
                ASpaceshipX* s = dynamic_cast<ASpaceshipX*>(OtherActor);
                if (s) {
                    s->beingFollowed = false;
                }
                else {
                    ASpaceshipY* b = dynamic_cast<ASpaceshipY*>(OtherActor);
                    if (b) {
                        b->beingFollowed = false;
                    }
                }

                TargetSpaceship = NULL;
            }
            SpaceshipsInsideFOV.Remove(OtherActor);
        }
    }
}
