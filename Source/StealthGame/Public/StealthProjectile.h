// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StealthProjectile.generated.h"


class UProjectileMovementComponent;
class USphereComponent;


UCLASS()
class AStealthProjectile : public AActor
{
	GENERATED_BODY()

protected:

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Projectile")
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	// Maximum available amount of dealt noises
	UPROPERTY(EditDefaultsOnly, Category=Noise)
	float MaxPossibleNoiseCount = 2;

public:
	AStealthProjectile();
	
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

private:
	int32 DealtNoises = 0;

	void MakeNoiseIfNecessary();
	void AddImpulseToPhysicsBody(AActor* OtherActor, UPrimitiveComponent* OtherComp);
};

