// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"



ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);
    
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

    PlayerInputComponent->BindAction(TEXT("FireBigProjectile"), IE_Pressed, this, &ATank::FireBigProjectile);
    PlayerInputComponent->BindAction(TEXT("FireSmallProjectile"), IE_Pressed, this, &ATank::FireConditionTrue);
    PlayerInputComponent->BindAction(TEXT("FireSmallProjectile"), IE_Released, this, &ATank::FireConditionFalse);
    PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &ATank::Dash);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (TankPlayerController)
    {
        FHitResult HitResult;
        TankPlayerController->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility,
            false,
            HitResult
        );
        
        RotateTurret(HitResult.ImpactPoint);
    }
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());	
}

void ATank::Move(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = Speed * Value * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;
    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
}

void ATank::Dash()
{
    UGameplayStatics::SpawnEmitterAtLocation(this, DestroyParticle, GetActorLocation(), GetActorRotation());

    const FVector  TeleportLocation = this->GetActorLocation() + this->GetActorRotation().Vector() * TeleportDistance;
    const FRotator TeleportRotation = this->GetActorRotation();

    TeleportTo(TeleportLocation, TeleportRotation, true, true);

    UGameplayStatics::SpawnEmitterAtLocation(this, DestroyParticle, GetActorLocation(), GetActorRotation());
}

