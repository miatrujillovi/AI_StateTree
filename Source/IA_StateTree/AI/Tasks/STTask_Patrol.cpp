// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/STTask_Patrol.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "Navigation/PathFollowingComponent.h"
#include "AI/AIAgentCharacter.h"

EStateTreeRunStatus FSTTask_Patrol::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FSTTask_PatrolInstanceData& Data = Context.GetInstanceData(*this);
	AAIController* AIController = Cast<AAIController>(Context.GetOwner());
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("PATROL: AIController is NULL"));
		return EStateTreeRunStatus::Failed;
	}
	
	/*UE_LOG(LogTemp, Warning, TEXT("PATROL: Moving toward %s"), *Target->GetName());
	
	const EPathFollowingRequestResult::Type Result = AIController->MoveToActor(Target, Data.AcceptanceRadius);
	UE_LOG(LogTemp, Warning, TEXT("PATROL: MoveToActor result = %d"), static_cast<int32>(Result));
	
	if (Result == EPathFollowingRequestResult::Failed)
	{
		UE_LOG(LogTemp, Error, TEXT("PATROL: Movement request failed"));
		return EStateTreeRunStatus::Failed;
	}*/
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_Patrol::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FSTTask_PatrolInstanceData& Data = Context.GetInstanceData(*this);
	AAIController* AIController = Cast<AAIController>(Context.GetOwner());
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("PATROL: AIController is NULL"));
		return EStateTreeRunStatus::Failed;
	}
	
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("PATROL: Pawn is NULL"));
		return EStateTreeRunStatus::Failed;
	}
	
	AActor* Target = Data.bGoingToA ? Data.PatrolPointA : Data.PatrolPointB;
	if (!Target)
	{
		UE_LOG(LogTemp, Error, TEXT("PATROL: Current target is NULL"));
		return EStateTreeRunStatus::Failed;
	}
	
	const float Distance = FVector::Dist(Pawn->GetActorLocation(), Target->GetActorLocation());
	if (Distance <= Data.AcceptanceRadius)
	{
		Data.bGoingToA = !Data.bGoingToA;

		Target = Data.bGoingToA ? Data.PatrolPointA : Data.PatrolPointB;
		
		if (!Target)
		{ return EStateTreeRunStatus::Failed; }
		
		UE_LOG(LogTemp, Warning, TEXT("PATROL: Reached point. Next target: %s"), *Target->GetName());
		
		const EPathFollowingRequestResult::Type Result = AIController->MoveToActor(Target, Data.AcceptanceRadius);
		UE_LOG(LogTemp, Warning, TEXT("PATROL: Next movement result = %d"), static_cast<int32>(Result));
		
		if (Result == EPathFollowingRequestResult::Failed)
		{
			return EStateTreeRunStatus::Failed;
		}
	}
	
	return EStateTreeRunStatus::Running;
}

void FSTTask_Patrol::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	if (AAIController* AIController = Cast<AAIController>(Context.GetOwner()))
	{
		AIController->StopMovement();
	}
}
