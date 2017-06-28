// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainingGrounds.h"
#include "ChooseNextWaypoint.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Running waypoint stuff"));
	return EBTNodeResult::Succeeded;
}


