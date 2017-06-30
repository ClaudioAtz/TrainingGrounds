// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainingGrounds.h"
#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolRoute.h"
#include "AIController.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();

	if (!ControlledPawn) {
		return EBTNodeResult::Failed;
	}
	
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (!PatrolRoute)
	{
		return EBTNodeResult::Failed;
	}

	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);

	auto PatrolPoints = PatrolRoute->GetPatrolPoints();

	if (PatrolPoints.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	BlackboardComp->SetValueAsObject(Waypoint.SelectedKeyName, PatrolPoints[Index]);

	Index++;

	int32 NewIndex = Index % PatrolPoints.Num();

	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NewIndex);
	

	return EBTNodeResult::Succeeded;
}


