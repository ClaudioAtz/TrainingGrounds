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

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn)
{
	FVector Min(0, -2000, 0);
	FVector Max(4000, 2000, 0);
	FBox Bounds(Min, Max);

	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FVector SpawnPoint = FMath::RandPointInBox(Bounds);
		CastSphere(&SpawnPoint, 300);
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
		Spawned->SetActorRelativeLocation(SpawnPoint);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}
}

bool ATile::CastSphere(const FVector* Location, float Radius)
{
	FHitResult HitResult;

	// Using same start and end which effectively makes this a Sphere Cast
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult, 
		*Location, 
		*Location, 
		FQuat::Identity, 
		ECollisionChannel::ECC_Camera, 
		FCollisionShape::MakeSphere(Radius)
	);

	FColor SphereColour = (bResult) ? FColor::Red : FColor::Green;

	DrawDebugSphere(
		GetWorld(),
		*Location,
		Radius,
		32,
		SphereColour,
		true
	);

	return bResult;
}
