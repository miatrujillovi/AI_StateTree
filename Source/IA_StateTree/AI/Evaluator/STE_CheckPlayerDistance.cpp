// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Evaluator/STE_CheckPlayerDistance.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "AI/AIAgentController.h"

void FSTE_CheckPlayerDistance::TreeStart(FStateTreeExecutionContext& Context) const
{
	FSTE_CheckPlayerDistanceInstanceData& Data = Context.GetInstanceData(*this);
	
	Data.DistanceToPlayer = 0.f;
}

void FSTE_CheckPlayerDistance::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FSTE_CheckPlayerDistanceInstanceData& Data = Context.GetInstanceData(*this);
	
	AAIAgentController* AIController = Cast<AAIAgentController>(Context.GetOwner());
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("CHECK PLAYER DISTANCE: AIController is NULL"));
	}
	
	AActor* Player = AIController->GetPlayer();
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("CHECK PLAYER DISTANCE: Controller has no player reference"));
	}
	
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		Data.DistanceToPlayer = -1.f;
		return;
	}
	
	Data.DistanceToPlayer = FVector::Dist(Pawn->GetActorLocation(), Player->GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("DISTANCE CHECK: Player=%s | Distance=%.2f"), *GetNameSafe(Data.Player), Data.DistanceToPlayer);
}

void FSTE_CheckPlayerDistance::TreeStop(FStateTreeExecutionContext& Context) const
{
}
