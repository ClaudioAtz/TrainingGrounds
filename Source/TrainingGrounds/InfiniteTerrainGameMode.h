// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TrainingGroundsGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"


// Enum for mission types
UENUM()
enum class EConquerCondition : uint8 { Stealth, Assassination, Rescue, None };

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

	UFUNCTION(BlueprintCallable, Category = "Score")
	void TileConquered();

	UFUNCTION(BlueprintCallable, Category = "Score")
	int GetNumberOfConqueredTiles() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	EConquerCondition GetCurrentConquerCondition() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void SetCurrentConquerCondition(EConquerCondition Condition);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	UActorPool* NavMeshBoundsVolumePool = nullptr;

private:
	void AddToPool(ANavMeshBoundsVolume* VolumeToAdd);

	// Current tile active conquer condition
	EConquerCondition CurrentConquerCondition = EConquerCondition::None;

	// Number of "rooms" you've been through
	int Score = 0;
};
