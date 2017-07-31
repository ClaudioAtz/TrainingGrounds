// Fill out your copyright notice in the Description page of Project Settings.
/**
* Class used to spawn objects that players can pick up.
*/
#pragma once

#include "GameFramework/Actor.h"
#include "ItemAltar.generated.h"

class UArrowComponent;
class AGun;
class UStaticMeshComponent;

UCLASS()
class TRAININGGROUNDS_API AItemAltar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemAltar();

	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<AGun> Item;

	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMeshComponent* Altar = nullptr;

	UPROPERTY(EditAnywhere, Category = "Setup")
	UArrowComponent* ItemArrow;

	UPROPERTY(EditAnywhere, Category = "Item")
	float ItemRotationSpeed = 100.f;

	UFUNCTION()
	TSubclassOf<AGun> GetItem() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



private:
	AGun* ItemMesh = nullptr;
};
