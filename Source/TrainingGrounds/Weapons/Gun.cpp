// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainingGrounds.h"
#include "Gun.h"
#include "BallProjectile.h"
#include "Animation/AnimInstance.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Hearing.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	SetRootComponent(FP_Gun);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	//FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	// So we can shoot straight away.
	LastFireTime = -ReloadTimeInSeconds;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL && !IsReloading())
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = FP_MuzzleLocation->GetComponentRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// spawn the projectile at the muzzle
			auto Projectile = World->SpawnActor<ABallProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			if (Projectile != nullptr && Tags.Num() > 0)
			{
				Projectile->SetTeamTag(Tags[0]);
			}


			LastFireTime = GetWorld()->GetTimeSeconds();

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
				UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), 1.0, this, 2000.f, FName("SOUND"));
			}

			// try and play a firing animation if specified
			if (AnimInstance1P != nullptr && FireAnimation1P != nullptr)
			{
				AnimInstance1P->Montage_Play(FireAnimation1P, 1.f);
			}
			else if (AnimInstance3P != nullptr && FireAnimation3P != nullptr)
			{
				AnimInstance3P->Montage_Play(FireAnimation3P, 1.f);
			}
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("NO PROJECTILE CLASS"));
	}
}

bool AGun::IsReloading() const
{
	return (GetWorld()->GetTimeSeconds() - LastFireTime) < ReloadTimeInSeconds;
}