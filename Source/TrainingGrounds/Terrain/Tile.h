// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "InfiniteTerrainGameMode.h"
#include "Tile.generated.h"


// Enum for mission types
UENUM(BlueprintType)
enum class EConquerCondition : uint8 { Assassination, Rescue, None };

USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;
};

class UActorPool;

UCLASS()
class TRAININGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* InPool);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500.f, float MinScale = 1, float MaxScale = 1);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void PlaceAiPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 50.f);

	UFUNCTION(BlueprintCallable, Category = "Conquest")
	void SetConqueredTile();

	UFUNCTION(BlueprintCallable, Category = "Conquest")
	bool IsTileConquered();

	UFUNCTION(BlueprintCallable, Category = "Conquest")
	void SetConquerCondition(EConquerCondition Condition);

	UFUNCTION(BlueprintCallable, Category = "Conquest")
	EConquerCondition GetConquerCondition();

	UFUNCTION()
	void OnPossessedEnemyDeath();

	UFUNCTION(BlueprintImplementableEvent, Category = "Conquest")
	void OnTileConquered();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector NavigationBoundsOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinSpawningExtent;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxSpawningExtent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


private:

	UActorPool* Pool = nullptr;
	AActor* NavMeshBoundsVolume = nullptr;

	// Array of references to all spawned actors in the tile, kept for cleaning up duties
	TArray < AActor* > SpawnedActors;

	// Number of enemies spawned
	int SpawnedEnemies = 0;

	int KilledEnemies = 0;

	// Tile Conquer Condition
	UPROPERTY(EditAnywhere, Category = "Conquest")
	EConquerCondition ConquerCondition = EConquerCondition::Assassination;

	bool bConquered = false;

	bool CastSphere(FVector Location, float Radius);
	bool FindEmptyLocation(FVector& OutEmptySpace, float Radius);
	void PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition);
	void PositionNavMeshBoundsVolume();

	void ReleaseNavMesh();
	void ReleaseActors();
	
};
