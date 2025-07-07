// Fill out your copyright notice in the Description page of Project Settings.


#include "RedLaser.h"
#include "SpaceshipX.h"
#include "SpaceshipY.h"

// Sets default values
ARedLaser::ARedLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    UStaticMeshComponent* Laser = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
    Laser->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> LaserVisualAsset(TEXT("StaticMesh'/Game/Assets/Utils/RedLaser.RedLaser'"));
    if (LaserVisualAsset.Succeeded())
    {
        Laser->SetStaticMesh(LaserVisualAsset.Object);
        Laser->SetRelativeLocation(FVector(0.0f, -600.0f, -100.0f));
        Laser->SetWorldScale3D(FVector(0.8f, 0.8f, 1.0f));
        FRotator rot = FRotator(90, 0, 90);
        Laser->SetWorldRotation(rot);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Failed"));
    }

    Laser->SetCollisionProfileName("Trigger");
    Laser->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARedLaser::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ARedLaser::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARedLaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (allDataSet) {
        FVector newPos = GetActorLocation() + velocity * 0.98 * DeltaTime;
        newPos.Z -= 1; // gravity acting
        SetActorLocation(newPos);
    }

    if (existingTime > timeToLive)
    {
        Destroy();
    }

    existingTime += DeltaTime;
}

void ARedLaser::SetSpeed(float s)
{
    speed = s;
}

void ARedLaser::SetDirection(FVector dir)
{
    direction = dir;
}

void ARedLaser::SetVelocity(FVector v)
{
    velocity = v;
}

void ARedLaser::AllDataSet(bool b)
{
    allDataSet = b;
}

void ARedLaser::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->GetClass()->IsChildOf(ASpaceshipX::StaticClass()) || OtherActor->GetClass()->IsChildOf(ASpaceshipY::StaticClass())) {
        Destroy();
    }
}