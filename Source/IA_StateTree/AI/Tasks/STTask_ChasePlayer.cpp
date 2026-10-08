// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/STTask_ChasePlayer.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"


EStateTreeRunStatus FSTTask_ChasePlayer::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FSTTask_ChasePlayerInstanceData& Data = Context.GetInstanceData(*this);
	AAIController* AIController = Cast<AAIController>(Context.GetOwner());
	if (!AIController)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (!Data.Player)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	//Move AI to the Player
	AIController->MoveToActor(Data.Player, Data.AcceptanceRadius);
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_ChasePlayer::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FSTTask_ChasePlayerInstanceData& Data = Context.GetInstanceData(*this);
	AAIController* AIController = Cast<AAIController>(Context.GetOwner());
	if (!AIController)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (!Data.Player)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	AIController->MoveToActor(Data.Player, Data.AcceptanceRadius);
	
	return EStateTreeRunStatus::Running;
}

void FSTTask_ChasePlayer::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	AAIController* AIController = Cast<AAIController>(Context.GetOwner());
	if (AIController)
	{
		AIController->StopMovement();
	}
	
	//return EStateTreeRunStatus::Succeeded;
}
