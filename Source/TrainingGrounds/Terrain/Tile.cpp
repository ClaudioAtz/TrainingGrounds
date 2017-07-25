// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainingGrounds.h"
#include "Tile.h"
#include "DrawDebugHelpers.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FVector SpawnPoint;
		float RandomScale = FMath::RandRange(MinScale, MaxScale);

		if (FindEmptyLocation(SpawnPoint, Radius * RandomScale))
		{
			float Rotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPoint, Rotation, RandomScale);
		}
	}
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector Location, float Rotation, float Scale)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(Location);
	Spawned->SetActorRotation(FRotator(0, Rotation, 0));
	Spawned->SetActorScale3D(FVector(Scale));
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

bool ATile::CastSphere(FVector Location, float Radius)
{
	FHitResult HitResult;

	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	// Using same start and end which effectively makes this a Sphere Cast
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult, 
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity, 
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	FColor SphereColour = (bResult) ? FColor::Red : FColor::Green;

	/*DrawDebugSphere(
		GetWorld(),
		GlobalLocation,
		Radius,
		32,
		SphereColour,
		true
	);*/

	return bResult;
}

bool ATile::FindEmptyLocation(FVector& OutEmptySpace, float Radius)
{
	FVector Min(0, -2000, 0);
	FVector Max(4000, 2000, 0);
	FBox Bounds(Min, Max);

	for (int i = 0; i < 100; i++)
	{
		FVector LocationResult = FMath::RandPointInBox(Bounds);
		if (!CastSphere(LocationResult, Radius))
		{
			OutEmptySpace = LocationResult;
			return true;
		}
	}

	return false;
}