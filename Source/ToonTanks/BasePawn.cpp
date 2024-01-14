// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Particles/ParticleSystem.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::HandleDestruction()
{
	if(DestroyParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DestroyParticle, GetActorLocation(),GetActorRotation());
	}
	if (DestroySound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DestroySound, GetActorLocation());
	}
	if (DestroyCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DestroyCameraShakeClass);
	}
	
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation(); 
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	TurretMesh->SetWorldRotation(LookAtRotation);
}

void ABasePawn::FireBigProjectile()
{
		if (ProjectileSpawnPoint)
		{
			auto Projectile = GetWorld()->SpawnActor<AProjectile>(BigProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
			Projectile->SetOwner(this);
		}
}

void ABasePawn::FireSmallProjectile()
{	
	if (FireCondition)
	{
		if (ProjectileSpawnPoint)
		{	
			auto Projectile = GetWorld()->SpawnActor<AProjectile>(SmallProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
			Projectile->SetOwner(this);
		}
	}
}

void ABasePawn::FireConditionTrue()
{
	FireCondition = true;
	GetWorldTimerManager().SetTimer(ProjectileSpawnTimerHandle, this, &ABasePawn::FireSmallProjectile, ProjectileSpawnRate, true);
}

void ABasePawn::FireConditionFalse()
{
	FireCondition = false;
	GetWorldTimerManager().SetTimer(ProjectileSpawnTimerHandle, this, &ABasePawn::FireSmallProjectile, ProjectileSpawnRate, true);
}

