// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GrassComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TRAININGGROUNDS_API UGrassComponent : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = Spawning)
	FBox SpawningExtents;

	UPROPERTY(EditDefaultsOnly, Category = Spawning)
	int SpawnCount;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void SpawnGrass();
};
