// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/STTask_ChasePlayer.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "Navigation/PathFollowingComponent.h"
#include "AI/AIAgentController.h"
#include "AI/AIAgentCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


EStateTreeRunStatus FSTTask_ChasePlayer::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FSTTask_ChasePlayerInstanceData& Data = Context.GetInstanceData(*this);
	
	//Initialize references once when entering Chase Player-------------------------------
	Data.AIController = Cast<AAIAgentController>(Context.GetOwner());
	if (!Data.AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("CHASE: AIController is NULL"));
		return EStateTreeRunStatus::Failed;
	}
	
	Data.AICharacter = Cast<AAIAgentCharacter>(Data.AIController->GetPawn());
	if (Data.AICharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("CHASE: AICharacter is NULL"));
		return EStateTreeRunStatus::Failed;
	}
	//Finalize assigning references ------------------------------------------------------
	
	AActor* Player = Data.AIController->GetPlayer();
	if (!Player)
	{
		UE_LOG(LogTemp, Error,
			TEXT("CHASE: Controller has no player reference"));
		return EStateTreeRunStatus::Failed;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("CHASE: Targeting player %s"), *Player->GetName());
	
	const EPathFollowingRequestResult::Type Result = Data.AIController->MoveToActor(Player, Data.AcceptanceRadius);
	UE_LOG(LogTemp, Warning, TEXT("CHASE: MoveToActor result = %d"), static_cast<int32>(Result));
	
	if (Data.AICharacter)
	{
		UCharacterMovementComponent* Movement = Data.AICharacter->GetCharacterMovement();

		UE_LOG(LogTemp, Warning,
			TEXT("AI DIAGNOSTICS: Location=%s | Velocity=%s | MovementMode=%d | MaxWalkSpeed=%.1f"),
			*Data.AICharacter->GetActorLocation().ToString(),
			*Data.AICharacter->GetVelocity().ToString(),
			Movement ? static_cast<int32>(Movement->MovementMode) : -1,
			Movement ? Movement->MaxWalkSpeed : -1.f);
	}
	
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
	
	AActor* Player = Data.AIController->GetPlayer();
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("CHASE: Controller has no player reference"));
		return EStateTreeRunStatus::Failed;
	}
	
	return EStateTreeRunStatus::Running;
}

void FSTTask_ChasePlayer::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FSTTask_ChasePlayerInstanceData& Data = Context.GetInstanceData(*this);
	
	if (Data.AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("TASK EXIT: %s"), *GetNameSafe(Data.AIController));
		Data.AIController->StopMovement();
	}
}
