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

	ItemArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FloatingItemArrow"));
	ItemArrow->SetupAttachment(Altar);
}

// Called when the game starts or when spawned
void AItemAltar::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (Item != nullptr)
	{
		SpawnedItem = GetWorld()->SpawnActor<AItem>(
			Item,
			ItemArrow->GetComponentLocation(),
			ItemArrow->GetComponentRotation(),
			ActorSpawnParams
			);
	}
	else if (Weapon != nullptr)
	{
		SpawnedItem = GetWorld()->SpawnActor<AItem>(
			Weapon,
			ItemArrow->GetComponentLocation(),
			ItemArrow->GetComponentRotation(),
			ActorSpawnParams
			);
	}

	// Make sure that the floating item follows the altar wherever it's spawned.
	SpawnedItem->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

// Called every frame
void AItemAltar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnedItem != nullptr)
	{
		SpawnedItem->AddActorLocalRotation(FRotator(0.f, ItemRotationSpeed * DeltaTime, 0.f));
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

void AItemAltar::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor != nullptr && OtherActor->ActorHasTag(FName("Player")))
	{
			switch (ItemType)
			{
				case EItemType::Quest :
				{
					bConsumed = true;
					if (SpawnedItem != nullptr)
					{
						OnItemCollected.Broadcast();
						SpawnedItem->Destroy();
					}
					Altar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}
				break;
				default:
				{
					UE_LOG(LogTemp, Warning, TEXT("COnsuming item"));
				}
				break;
			}
		
	}
}

