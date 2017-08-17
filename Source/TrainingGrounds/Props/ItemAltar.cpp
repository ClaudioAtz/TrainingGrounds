// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainingGrounds.h"
#include "ItemAltar.h"
#include "Item.h"
#include "Components/ArrowComponent.h"
#include "Weapons/Gun.h"


// Sets default values
AItemAltar::AItemAltar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Altar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Altar"));
	Altar->bCastDynamicShadow = false;
	Altar->CastShadow = true;
	Altar->RelativeLocation = FVector(0.f, 0.f, 0.f);

	ItemArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FloatingItemArrow"));
	ItemArrow->SetupAttachment(Altar);
}

// Called when the game starts or when spawned
void AItemAltar::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	if (Item != nullptr)
	{
		ItemMesh = GetWorld()->SpawnActor<AItem>(
			Item,
			ItemArrow->GetComponentLocation(),
			ItemArrow->GetComponentRotation(),
			ActorSpawnParams
			);
	}
	else if (Weapon != nullptr)
	{
		ItemMesh = GetWorld()->SpawnActor<AItem>(
			Weapon,
			ItemArrow->GetComponentLocation(),
			ItemArrow->GetComponentRotation(),
			ActorSpawnParams
			);
	}

}

// Called every frame
void AItemAltar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ItemMesh != nullptr)
	{
		ItemMesh->AddActorLocalRotation(FRotator(0.f, ItemRotationSpeed * DeltaTime, 0.f));
	}
}

TSubclassOf<AGun> AItemAltar::GetWeapon() const
{
	return Weapon;
}

TSubclassOf<AItem> AItemAltar::GetItem() const
{
	return Item;
}

