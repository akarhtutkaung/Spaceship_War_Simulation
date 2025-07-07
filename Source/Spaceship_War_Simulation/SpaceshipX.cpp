// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceshipX.h"
#include "SpaceshipY.h"
#include "RedLaser.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASpaceshipX::ASpaceshipX()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create and position a mesh component
    Spaceship = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
    Spaceship->SetupAttachment(RootComponent);
    Spaceship->SetCollisionProfileName("Trigger");
    Spaceship->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABaseSpaceship::OnOverlapBegin);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SpaceshipVisualAsset(TEXT("StaticMesh'/Game/Assets/Spaceships/mesh/spaceship5.spaceship5'"));
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
    ExplosionSystem->SetRelativeLocation(FVector(0.0f, -7.0f, 0.0f));
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
    SpringArmComp->SetRelativeLocationAndRotation(FVector(7.6f, -16.0f, 0.0f), FRotator(0.0f, 100.0f, -90.0f));
    SpringArmComp->TargetArmLength = 400.f;

    
}

// Called when the game starts or when spawned
void ASpaceshipX::BeginPlay()
{
	Super::BeginPlay();

    cone->EnemyBaseClass = ASpaceshipY::StaticClass();
    cone->MySpaceship = this;

    NormalTimeToChangeVelocity = FMath::RandRange(2.0, 5.0);

}

// Called every frame
void ASpaceshipX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if (HP > 0) {
        if (!beingFollowed && cone->TargetSpaceship) { // Only if not being follow, try to follow other
            // If enemy is inside FOV
            ASpaceshipY* TargetSpaceship = dynamic_cast<ASpaceshipY*>(cone->TargetSpaceship);
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

void ASpaceshipX::CreateLaser()
{
    FVector dir = GetTransform().GetUnitAxis(EAxis::Y);
    FVector loc = GetActorLocation() + dir * 500;

    ARedLaser* laser = GetWorld()->SpawnActor<ARedLaser>(ARedLaser::StaticClass(), loc, GetActorRotation());
    if (laser) {
        FVector forwardY = GetTransform().GetUnitAxis(EAxis::Y);
        forwardY.Normalize();
        FVector defaultLaserVelocity = forwardY * laserSpeed * LASER_SPEED_MULTIPLIER;
        
        laser->SetVelocity(defaultLaserVelocity);
        laser->AllDataSet(true);
    }
}
