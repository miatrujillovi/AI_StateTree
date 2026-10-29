// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/STTask_Patrol.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "Navigation/PathFollowingComponent.h"
#include "AI/AIAgentCharacter.h"
#include "AI/AIAgentController.h"

EStateTreeRunStatus FSTTask_Patrol::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FSTTask_PatrolInstanceData& Data = Context.GetInstanceData(*this);
	
	//Initialize references once when entering Patrol--------------------------------------
	Data.AIController = Cast<AAIAgentController>(Context.GetOwner());
	if (!Data.AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("PATROL: AIController is NULL"));
	}
	
	Data.AICharacter = Cast<AAIAgentCharacter>(Data.AIController->GetPawn());
	if (!Data.AICharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("PATROL: Could not get AI character"));
	}
	
	Data.PatrolPointA = Data.AICharacter->PatrolPointA;
	Data.PatrolPointB = Data.AICharacter->PatrolPointB;
	if (!Data.PatrolPointA || !Data.PatrolPointB)
	{
		UE_LOG(LogTemp, Error, TEXT("PATROL: Character patrol points are not assigned"));
		return EStateTreeRunStatus::Failed;
	}
	//Finalize assigning references ------------------------------------------------------
	
	AActor* Target = Data.bGoingToA ? Data.PatrolPointA : Data.PatrolPointB;
	if (!Target)
	{
		UE_LOG(LogTemp, Error, TEXT("PATROL: Character patrol points are not assigned"));
		return EStateTreeRunStatus::Failed;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("PATROL: Moving toward %s"), *Target->GetName());
	
	const EPathFollowingRequestResult::Type Result = Data.AIController->MoveToActor(Target, Data.AcceptanceRadius);
	if (Result == EPathFollowingRequestResult::Failed)
	{
		UE_LOG(LogTemp, Error, TEXT("PATROL: Movement request failed"));
		return EStateTreeRunStatus::Failed;
	}
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_Patrol::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FSTTask_PatrolInstanceData& Data = Context.GetInstanceData(*this);
	
	APawn* Pawn = Data.AIController->GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("PATROL: Pawn is NULL"));
		return EStateTreeRunStatus::Failed;
	}
	
	AActor* Target = Data.bGoingToA ? Data.PatrolPointA : Data.PatrolPointB;
	if (!Target)
	{
		UE_LOG(LogTemp, Error, TEXT("PATROL: Character patrol points are not assigned"));
		return EStateTreeRunStatus::Failed;
	}
	
	const float Distance = FVector::Dist(Pawn->GetActorLocation(), Target->GetActorLocation());
	if (Distance <= Data.AcceptanceRadius)
	{
		Data.bGoingToA = !Data.bGoingToA;

		Target = Data.bGoingToA ? Data.PatrolPointA : Data.PatrolPointB;
		if (!Target)
		{
			return EStateTreeRunStatus::Failed;
		}
		
		UE_LOG(LogTemp, Warning, TEXT("PATROL: Reached point. Next target: %s"), *Target->GetName());
		
		const EPathFollowingRequestResult::Type Result = Data.AIController->MoveToActor(Target, Data.AcceptanceRadius);
		if (Result == EPathFollowingRequestResult::Failed)
		{
			UE_LOG(LogTemp, Error, TEXT("PATROL: Movement request failed"));
			return EStateTreeRunStatus::Failed;
		}
	}
	
	return EStateTreeRunStatus::Running;
}

void FSTTask_Patrol::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FSTTask_PatrolInstanceData& Data = Context.GetInstanceData(*this);
	if (Data.AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("TASK EXIT: %s"), *GetNameSafe(Data.AIController));
		Data.AIController->StopMovement();
	}
}
