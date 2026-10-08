// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Evaluator/STE_CheckPlayerDistance.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"

void FSTE_CheckPlayerDistance::TreeStart(FStateTreeExecutionContext& Context) const
{
	FSTE_CheckPlayerDistanceInstanceData& Data = Context.GetInstanceData(*this);
	
	Data.DistanceToPlayer = 0.f;
}

void FSTE_CheckPlayerDistance::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FSTE_CheckPlayerDistanceInstanceData& Data = Context.GetInstanceData(*this);
	
	if (!Data.Player)
	{
		Data.DistanceToPlayer = -1.f;
		return;
	}
	
	AAIController* AIController = Cast<AAIController>(Context.GetOwner());
	if (!AIController)
	{
		Data.DistanceToPlayer = -1.f;
		return;
	}
	
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		Data.DistanceToPlayer = -1.f;
		return;
	}
	
	Data.DistanceToPlayer = FVector::Dist(Pawn->GetActorLocation(), Data.Player->GetActorLocation());
}

void FSTE_CheckPlayerDistance::TreeStop(FStateTreeExecutionContext& Context) const
{
}
