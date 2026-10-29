// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/STTask_TacticalPosition.h"

#include "AI/AIAgentController.h"

#include "StateTreeExecutionContext.h"

#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

#include "Navigation/PathFollowingComponent.h"


EStateTreeRunStatus FSTTask_TacticalPosition::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	
	//Initialize references once when entering Tactical Position--------------------------
	Data.AIController = Cast<AAIAgentController>(Context.GetOwner());
	if (!Data.AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("TACTICAL: AIController is NULL"));
		return EStateTreeRunStatus::Failed;
	}
	//Finalize assigning references ------------------------------------------------------
	
	if (!Data.TacticalQuery)
	{
		UE_LOG(LogTemp, Error, TEXT("TACTICAL: TacticalQuery is not assigned"));
		return EStateTreeRunStatus::Failed;
	}
	
	if (!Data.AIController->GetPawn())
	{
		UE_LOG(LogTemp, Error, TEXT("TACTICAL: Controller has no possessed pawn"));
		return EStateTreeRunStatus::Failed;
	}
	
	Data.bHasTargetLocation = false;
	Data.QueryInstance = nullptr;
	
	Data.AIController->StopMovement();
	
	Data.QueryInstance = UEnvQueryManager::RunEQSQuery(Data.AIController, Data.TacticalQuery, Data.AIController,
											EEnvQueryRunMode::SingleResult,
											UEnvQueryInstanceBlueprintWrapper::StaticClass());
	
	if (!Data.QueryInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("TACTICAL: Failed to start EQS query"));
		return EStateTreeRunStatus::Failed;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("TACTICAL: EQS query started"));
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_TacticalPosition::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	
	APawn* AIPawn = Data.AIController->GetPawn();
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("TACTICAL: Controller has no possessed pawn"));
		return EStateTreeRunStatus::Failed;
	}
	
	// Stage 1: Wait for EQS to finish and retrieve the best location.
	if (!Data.bHasTargetLocation)
	{
		if (!Data.QueryInstance)
		{
			UE_LOG(LogTemp, Error, TEXT("TACTICAL: EQS query instance is NULL"));
			return EStateTreeRunStatus::Failed;
		}

		const FEnvQueryResult* QueryResult = Data.QueryInstance->GetQueryResult();
		if (!QueryResult || !QueryResult->IsFinished())
		{
			return EStateTreeRunStatus::Running;
		}

		if (!QueryResult->IsSuccessful())
		{
			/*UE_LOG(LogTemp, Error, TEXT("TACTICAL: EQS query failed. " "Items: %d, Aborted: %s"),
											QueryResult->Items.Num(),
											QueryResult->IsAborted() ? TEXT("YES") : TEXT("NO"));
			
			UE_LOG(LogTemp, Error, TEXT("TACTICAL: Check the EQS generator, " "context, and tests for rejected candidates."));*/
			return EStateTreeRunStatus::Failed;
		}
		
		const TArray<FVector> Locations = Data.QueryInstance->GetResultsAsLocations();
		if (Locations.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("TACTICAL: EQS returned no locations"));
			return EStateTreeRunStatus::Failed;
		}
		
		Data.TargetLocation = Locations[0];
		Data.bHasTargetLocation = true;

		UE_LOG(LogTemp, Warning, TEXT("TACTICAL: Selected position: %s"), *Data.TargetLocation.ToString());
		
		// Stage 2: Request movement to the selected position.
		const EPathFollowingRequestResult::Type MoveResult = Data.AIController->MoveToLocation(Data.TargetLocation, Data.AcceptanceRadius, true);

		if (MoveResult == EPathFollowingRequestResult::Failed)
		{
			UE_LOG(LogTemp, Error, TEXT("TACTICAL: MoveToLocation request failed"));
			return EStateTreeRunStatus::Failed;
		}

		if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			UE_LOG(LogTemp, Warning, TEXT("TACTICAL: Already at tactical position"));
			return EStateTreeRunStatus::Succeeded;
		}

		return EStateTreeRunStatus::Running;
	}
	
	// Stage 3: Wait until movement completes.
	const float Distance = FVector::Dist(Data.AIController->GetPawn()->GetActorLocation(), Data.TargetLocation);

	if (Distance <= Data.AcceptanceRadius)
	{
		UE_LOG(LogTemp, Warning, TEXT("TACTICAL: Reached tactical position"));
		return EStateTreeRunStatus::Succeeded;
	}

	UPathFollowingComponent* PathFollowing = Data.AIController->GetPathFollowingComponent();
	if (PathFollowing && PathFollowing->GetStatus() == EPathFollowingStatus::Idle)
	{
		UE_LOG(LogTemp, Warning, TEXT("TACTICAL: Movement stopped before reaching target. " "Distance remaining: %.1f"), Distance);
		return EStateTreeRunStatus::Failed;
	}
	
	return EStateTreeRunStatus::Running;
}

void FSTTask_TacticalPosition::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	
	UE_LOG(LogTemp, Warning, TEXT("TACTICAL: EXITED"));
	
	if (Data.AIController)
	{
		Data.AIController->StopMovement();
	}

	Data.QueryInstance = nullptr;
	Data.bHasTargetLocation = false;
}
