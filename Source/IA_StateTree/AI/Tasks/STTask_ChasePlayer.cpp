// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/STTask_ChasePlayer.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "Navigation/PathFollowingComponent.h"


EStateTreeRunStatus FSTTask_ChasePlayer::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FSTTask_ChasePlayerInstanceData& Data = Context.GetInstanceData(*this);
	AAIController* AIController = Cast<AAIController>(Context.GetOwner());
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("CHASE: AIController is NULL"));
		return EStateTreeRunStatus::Failed;
	}
	
	if (!Data.Player)
	{
		UE_LOG(LogTemp, Error, TEXT("CHASE: Data.Player is NULL. Check the State Tree binding."));
		return EStateTreeRunStatus::Failed;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("CHASE: Targeting player %s"), *Data.Player->GetName());
	
	const EPathFollowingRequestResult::Type Result = AIController->MoveToActor(Data.Player, Data.AcceptanceRadius);
	UE_LOG(LogTemp, Warning, TEXT("CHASE: MoveToActor result = %d"), static_cast<int32>(Result));
	
	if (Result == EPathFollowingRequestResult::Failed)
	{
		UE_LOG(LogTemp, Error, TEXT("CHASE: Movement request failed"));
		return EStateTreeRunStatus::Failed;
	}
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_ChasePlayer::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FSTTask_ChasePlayerInstanceData& Data = Context.GetInstanceData(*this);
	
	if (!Data.Player)
	{
		UE_LOG(LogTemp, Error, TEXT("CHASE: Player became NULL"));
		return EStateTreeRunStatus::Failed;
	}
	
	return EStateTreeRunStatus::Running;
}

void FSTTask_ChasePlayer::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	if (AAIController* AIController = Cast<AAIController>(Context.GetOwner()))
	{
		AIController->StopMovement();
	}
}
