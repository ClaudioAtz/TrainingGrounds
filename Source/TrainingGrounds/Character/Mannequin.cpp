// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainingGrounds.h"
#include "Mannequin.h"
#include "Components/ArrowComponent.h"
#include "Weapons/Gun.h"
#include "Props/ItemAltar.h"

// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();

	// Equip default weapon
	EquipWeapon(GunBlueprint);

	if (InputComponent != nullptr)
	{
		InputComponent->BindAxis("Fire", this, &AMannequin::Fire);
	}
}

void AMannequin::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	CleanUp();
}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMannequin::UnPossessed()
{
	Super::UnPossessed();

	OnMannequinDeath.Broadcast();

	if (Gun != nullptr) 
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
}

void AMannequin::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting View target"));
		auto PlayerController = Cast < APlayerController > (NewController);

		if (PlayerController != nullptr)
		{
			PlayerController->bAutoManageActiveCameraTarget = false;
			PlayerController->SetViewTargetWithBlend(this);
		}

	}
}

void AMannequin::Fire(float Value)
{
	if (Gun != nullptr)
	{
		if (Value != 0.f || !IsPlayerControlled())
		{
			Gun->OnFire();
		}
		
	}
}

void AMannequin::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor != nullptr)
	{
		if(OtherActor->ActorHasTag(FName("Item")))
		{
			UE_LOG(LogTemp, Warning, TEXT("######## TOUCHING ITEM #####"));
			auto ItemAltar = Cast<AItemAltar>(OtherActor);

			if (ItemAltar != nullptr)
			{
				auto Item = ItemAltar->GetWeapon();
				if (Item != nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("Picked up item %s"), *Item->GetName());
					EquipWeapon(Item);
				}
			}
		}
	}
}

void AMannequin::EquipWeapon(TSubclassOf<class AGun> Weapon)
{
	if (Weapon == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No GunBP present"));
		return;
	}

	if (Gun != nullptr)
	{
		Gun->Destroy();
	}

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	Gun = GetWorld()->SpawnActor<AGun>(Weapon);
	// Tag the gun with the right "team"
	Gun->Tags.Push(Tags[0]);

	if (IsPlayerControlled())
	{
		Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	else
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}

	Gun->AnimInstance1P = Mesh1P->GetAnimInstance();
	Gun->AnimInstance3P = GetMesh()->GetAnimInstance();
}

void AMannequin::CleanUp()
{
	if (Gun != nullptr)
	{
		Gun->Destroy();
	}
}