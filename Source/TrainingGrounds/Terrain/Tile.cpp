// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainingGrounds.h"
#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinSpawningExtent = FVector(0, -2000, 0);
	MaxSpawningExtent = FVector(4000, 2000, 0);

	NavigationBoundsOffset = FVector(2000, 0, 0);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (NavMeshBoundsVolume != nullptr && Pool != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Returned actor %s"), *NavMeshBoundsVolume->GetName());
		Pool->Return(NavMeshBoundsVolume);
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATile::PlaceAiPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	// Immediately return if no class has been passed.
	if (ToSpawn == nullptr)
	{
		return;
	}

	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = 1;

		if (FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale))
		{
			APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
			Spawned->SetActorRelativeLocation(SpawnPosition.Location);
			Spawned->SetActorRotation(FRotator(0, FMath::RandRange(-180.f, 180.f), 0));
			Spawned->SpawnDefaultController();
			Spawned->Tags.Add(FName("Enemy"));
			Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		}
	}
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	// Immediately return if no class has been passed.
	if (ToSpawn == nullptr)
	{
		return;
	}

	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);

		if (FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale))
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition)
{
	if (ToSpawn == nullptr)
	{
		return;
	}
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
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
	FBox Bounds(MinSpawningExtent, MaxSpawningExtent);

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

void ATile::SetPool(UActorPool* InPool)
{
	Pool = InPool;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Checked out actor %s"), *NavMeshBoundsVolume->GetName());
		NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
		GetWorld()->GetNavigationSystem()->Build();	
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to checkout NavMesh from NavMeshPool"));
	}
}

void ATile::SetConqueredTile()
{
	bConquered = true;
}

bool ATile::IsTileConquered()
{
	return bConquered;
}