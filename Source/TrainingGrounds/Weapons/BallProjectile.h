// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "BallProjectile.generated.h"

UCLASS(config=Game)
class ABallProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

public:
	ABallProjectile();

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool IsFriendlyFire() const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SetTeamTag(FName Tag) { TeamTag = Tag; }

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	FName GetTeamTag() const { return TeamTag; }

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool ShouldApplyDamage(AActor* OtherActor) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category ="Gameplay")
	bool bFriendlyFire = false;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	FName TeamTag;
};

