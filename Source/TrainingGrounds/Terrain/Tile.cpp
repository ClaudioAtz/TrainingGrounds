// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainingGrounds.h"
#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "Character/Mannequin.h"
#include "Props/ItemAltar.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	Super::EndPlay(EndPlayReason);
	ReleaseActors();
	ReleaseNavMesh();
}

void ATile::PlaceAiPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	// Immediately return if no class has been passed.
	if (ToSpawn == nullptr)
	{
		return;
	}

	SpawnedEnemies = 0;

	int RandomEnemies = FMath::RandRange(MinSpawn, MaxSpawn);

	for (size_t i = 0; i < RandomEnemies; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = 1;

		if (FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale))
		{
			FRotator Rotation(0, SpawnPosition.Rotation, 0);
			APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPosition.Location, Rotation);

			if (Spawned)
			{
				Spawned->SpawnDefaultController();
				Spawned->Tags.Add(FName("Enemy"));
				Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
				SpawnedActors.Push(Spawned);
			}

			auto Mannequin = Cast<AMannequin>(Spawned);
			if (Mannequin != nullptr)
			{
				SpawnedEnemies++;
				Mannequin->OnMannequinDeath.AddUniqueDynamic(this, &ATile::OnPossessedEnemyDeath);
			}
		}
	}
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale, bool QuestItem)
{
	// Immediately return if no class has been passed.
	if (ToSpawn == nullptr)
	{
		return;
		UE_LOG(LogTemp, Error, TEXT("PlaceActors called with no actor."));
	}

	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);

		if (FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale))
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			auto Actor = PlaceActor(ToSpawn, SpawnPosition);
			if (Actor != nullptr && QuestItem == true)
			{
				auto Altar = Cast<AItemAltar>(Actor);
				if (Altar != nullptr)
				{
					Altar->OnItemCollected.AddUniqueDynamic(this, &ATile::OnQuestItemCollected);
				}
			}
		}
	}
}

AActor* ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition)
{
	if (ToSpawn == nullptr)
	{
		return nullptr;
	}

	FRotator Rotation(0, SpawnPosition.Rotation, 0);

	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnPosition.Location, Rotation);
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	SpawnedActors.Push(Spawned);

	return Spawned;
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
/*
	FColor SphereColour = (bResult) ? FColor::Red : FColor::Green;

	DrawDebugSphere(
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

	for (int i = 0; i < 5; i++)
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
	if (Pool == nullptr)
	{
		return;
	}

	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume != nullptr)
	{
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

void ATile::ReleaseNavMesh()
{
	if (NavMeshBoundsVolume != nullptr && Pool != nullptr)
	{
		Pool->Return(NavMeshBoundsVolume);

		//NavMeshBoundsVolume = nullptr;
		//Pool = nullptr;
	}
}

void ATile::ReleaseActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if(Actor != nullptr)
		{
			Actor->Destroy();
		}
	}
}

void ATile::SetConquerCondition(EConquerCondition Condition)
{
	ConquerCondition = Condition;

	if (ConquerCondition == EConquerCondition::Assassination)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASSASS COND"));
	}
	if (ConquerCondition == EConquerCondition::Rescue)
	{
		UE_LOG(LogTemp, Warning, TEXT("RESCUE COND"));
	}
}

EConquerCondition ATile::GetConquerCondition()
{
	return ConquerCondition;
}

void ATile::OnPossessedEnemyDeath()
{
	KilledEnemies++;

	if (KilledEnemies >= SpawnedEnemies && ConquerCondition == EConquerCondition::Assassination)
	{
		OnTileConquered();
	}
}

void ATile::OnQuestItemCollected()
{
	QuestItemCollected++;

	UE_LOG(LogTemp, Warning, TEXT("Q Collected : %i"), QuestItemCollected);

	if (QuestItemCollected >= MaxQuestItems && ConquerCondition == EConquerCondition::Rescue)
	{
		OnTileConquered();
	}
}