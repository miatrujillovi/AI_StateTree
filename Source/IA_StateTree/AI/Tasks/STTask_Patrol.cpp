// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/STTask_Patrol.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FSTTask_Patrol::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FSTTask_PatrolInstanceData& Data = Context.GetInstanceData(*this);
	AAIController* AIController = Cast<AAIController>(Context.GetOwner());
	if (!AIController)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	AActor* Target = Data.bGoingToA ? Data.PatrolPointA : Data.PatrolPointB;
	if (!Target)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	//Move AI to one of the Points
	AIController->MoveToActor(Target, Data.AcceptanceRadius);
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_Patrol::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FSTTask_PatrolInstanceData& Data = Context.GetInstanceData(*this);
	AAIController* AIController = Cast<AAIController>(Context.GetOwner());
	if (!AIController)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	AActor* Target = Data.bGoingToA ? Data.PatrolPointA : Data.PatrolPointB;
	if (!Target)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	const float Distance = FVector::Dist(Pawn->GetActorLocation(), Target->GetActorLocation());
	if (Distance <= Data.AcceptanceRadius)
	{
		Data.bGoingToA = !Data.bGoingToA;

		Target = Data.bGoingToA ? Data.PatrolPointA : Data.PatrolPointB;

		AIController->MoveToActor(Target, Data.AcceptanceRadius);
	}
	
	return EStateTreeRunStatus::Running;
}

void FSTTask_Patrol::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	AAIController* AIController = Cast<AAIController>(Context.GetOwner());
	if (AIController)
	{
		AIController->StopMovement();
	}
	
	//return EStateTreeRunStatus::Succeeded;
}
