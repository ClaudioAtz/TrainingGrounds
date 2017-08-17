// Fill out your copyright notice in the Description page of Project Settings.
/**
* Class used to spawn objects that players can pick up.
*/
#pragma once

#include "GameFramework/Actor.h"
#include "ItemAltar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemDelegate);

// Enum for altar type
UENUM(BlueprintType)
enum class EItemType : uint8 { Weapon, Consumable, Quest };

class UArrowComponent;
class AGun;
class UStaticMeshComponent;
class AItem;
class USkeletalMeshComponent;

UCLASS()
class TRAININGGROUNDS_API AItemAltar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemAltar();

	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<AGun> Weapon;

	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<AItem> Item;

	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMeshComponent* Altar = nullptr;

	UPROPERTY(EditAnywhere, Category = "Setup")
	UArrowComponent* ItemArrow;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float ItemRotationSpeed = 100.f;

	UPROPERTY(EditAnywhere, Category = "Item")
	EItemType ItemType;

	UFUNCTION()
	TSubclassOf<AGun> GetWeapon() const;

	UFUNCTION()
	TSubclassOf<AItem> GetItem() const;

	// Delegate to notify if an item has been collected
	FItemDelegate OnItemCollected;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	AItem* SpawnedItem = nullptr;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	bool bConsumed = false;

};
