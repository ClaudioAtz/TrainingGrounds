// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TrainingGroundsGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

class ANavMeshBoundsVolume;
class UActorPool;

/**
 * 
 */
UCLASS()
class TRAININGGROUNDS_API AInfiniteTerrainGameMode : public ATrainingGroundsGameMode
{
	GENERATED_BODY()
	
public:
	AInfiniteTerrainGameMode();

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void PopulateBoundsVolumePool();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	UActorPool* NavMeshBoundsVolumePool = nullptr;

private:
	void AddToPool(ANavMeshBoundsVolume* VolumeToAdd);
	
};
