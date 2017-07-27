// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainingGrounds.h"
#include "InfiniteTerrainGameMode.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(FName("Nav Mesh bounds volume Pool"));	
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	auto NavVolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());

	while (NavVolumeIterator)
	{
		AddToPool(*NavVolumeIterator);
		++NavVolumeIterator;
	}
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume* VolumeToAdd)
{
	UE_LOG(LogTemp, Warning, TEXT("FInd nav mesh %s"), *VolumeToAdd->GetName());

	if (NavMeshBoundsVolumePool != nullptr && VolumeToAdd != nullptr)
	{
		NavMeshBoundsVolumePool->Add(VolumeToAdd);
	}
}

void AInfiniteTerrainGameMode::TileConquered()
{
	Score++;
	UE_LOG(LogTemp, Warning, TEXT("Tile conquered. Number of conquered tiles : %i"), Score);
}

int AInfiniteTerrainGameMode::GetNumberOfConqueredTiles()
{
	return Score;
}