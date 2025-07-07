// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceshipY.h"
#include "SpaceshipX.h"
#include "GreenLaser.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASpaceshipY::ASpaceshipY()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create and position a mesh component
    Spaceship = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
    Spaceship->SetupAttachment(RootComponent);
    Spaceship->SetCollisionProfileName("Trigger");
    Spaceship->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABaseSpaceship::OnOverlapBegin);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SpaceshipVisualAsset(TEXT("StaticMesh'/Game/Assets/Spaceships/mesh/spaceship8.spaceship8'"));
    if (SpaceshipVisualAsset.Succeeded())
    {
        Spaceship->SetStaticMesh(SpaceshipVisualAsset.Object);
        Spaceship->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
        Spaceship->SetWorldScale3D(FVector(100.0f));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Failed"));
    }

    ExplosionSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionParticles"));
    ExplosionSystem->SetupAttachment(Spaceship);
    ExplosionSystem->bAutoActivate = false;
    ExplosionSystem->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    ExplosionSystem->SetWorldScale3D(FVector(0.1f));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionAsset(TEXT("ParticleSystem'/Game/M5VFXVOL2/Particles/Explosion/Fire_Exp_00.Fire_Exp_00'"));
    if (ExplosionAsset.Succeeded())
    {
        ExplosionSystem->SetTemplate(ExplosionAsset.Object); 
    }

    // ===========================================================================
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    
    SpringArmComp->SetupAttachment(Spaceship);
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->SetWorldScale3D(FVector(0.05f));
    SpringArmComp->SetRelativeLocationAndRotation(FVector(5.0f, -2.6f, 0.0f), FRotator(0.0f, 110.0f, -90.0f));
    SpringArmComp->TargetArmLength = 400.f;
}

// Called when the game starts or when spawned
void ASpaceshipY::BeginPlay()
{
    Super::BeginPlay();

    cone->EnemyBaseClass = ASpaceshipX::StaticClass();
    cone->MySpaceship = this;

    NormalTimeToChangeVelocity = FMath::RandRange(2.0, 5.0);

}

// Called every frame
void ASpaceshipY::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (HP > 0) {
        if (!beingFollowed && cone->TargetSpaceship) { // Only if not being follow, try to follow other
            // If enemy is inside FOV
            ASpaceshipX* TargetSpaceship = dynamic_cast<ASpaceshipX*>(cone->TargetSpaceship);
            if (TargetSpaceship && TargetSpaceship->HP >= 0) {
                FollowTarget(TargetSpaceship);
                // Shoot laser
                if (prevLaserTime >= LASER_SHOT_INTERVAL) {
                    CreateLaser();
                    prevLaserTime = 0;
                }

                if (prevLaserTime < LASER_SHOT_INTERVAL) {
                    prevLaserTime += DeltaTime;
                }
            }
        }

        CheckBounds(DeltaTime);
        Move(DeltaTime);
        UpdateVelocity(DeltaTime);
    }
}

void ASpaceshipY::CreateLaser()
{
    FVector forwardY = GetTransform().GetUnitAxis(EAxis::Y);
    FVector forwardX = GetTransform().GetUnitAxis(EAxis::X);

    FVector locR = GetActorLocation() + (forwardY * 700) - (forwardX * 140);
    AGreenLaser* laserRight = GetWorld()->SpawnActor<AGreenLaser>(AGreenLaser::StaticClass(), locR, GetActorRotation());

    FVector locL = GetActorLocation() + (forwardY * 700) + (forwardX * 140);
    AGreenLaser* laserLeft = GetWorld()->SpawnActor<AGreenLaser>(AGreenLaser::StaticClass(), locL, GetActorRotation());

    forwardY.Normalize();
    FVector defaultLaserVelocity = forwardY * laserSpeed * LASER_SPEED_MULTIPLIER;
    if (laserRight) {
        laserRight->SetVelocity(velocity + defaultLaserVelocity);
        laserRight->AllDataSet(true);
    }
    if (laserLeft) {
        laserLeft->SetVelocity(velocity + defaultLaserVelocity);
        laserLeft->AllDataSet(true);
    }
}
