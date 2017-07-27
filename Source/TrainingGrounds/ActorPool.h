// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRAININGGROUNDS_API UActorPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorPool();	
	
	// Method to get an actor from the pull
	AActor* Checkout();

	// Method to return an actor to the pool once finished using it
	void Return(AActor* ActorToReturn);

	// Method used to populate the pool
	void Add(AActor* ActorToAdd);

private:

	TArray < AActor* > Pool;
};
