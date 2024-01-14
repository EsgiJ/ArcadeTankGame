// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	void HandleDestruction();

protected:

	void RotateTurret(FVector LookAtTarget);
	void FireBigProjectile();
	void FireSmallProjectile();
	void StopAutoFire();
	void FireConditionTrue();
	void FireConditionFalse();
	

	UPROPERTY(EditAnywhere,Category = "Combat")
	class UParticleSystem* DestroyParticle;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> BigProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> SmallProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* DestroySound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> DestroyCameraShakeClass;

	FTimerHandle ProjectileSpawnTimerHandle;
	float ProjectileSpawnRate = 0.05;

	bool FireCondition;
};

