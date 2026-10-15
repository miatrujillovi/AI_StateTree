// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Evaluator/STE_CheckPlayerPerception.h"
#include "AI/AIAgentController.h"
#include "StateTreeExecutionContext.h"


void FSTE_CheckPlayerPerception::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FSTE_CheckPlayerPerceptionInstanceData& Data = Context.GetInstanceData(*this);
	
	AAIAgentController* AIController = Cast<AAIAgentController>(Context.GetOwner());
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("CHECK PLAYER PERCEPTION: AIController is NULL"));
	}
	
	Data.Player = AIController->GetPlayer();
	if (!Data.Player)
	{
		UE_LOG(LogTemp, Error, TEXT("CHECK PLAYER PERCEPTION: Controller has no player reference"));
	}
	
	Data.bCanSeePlayer = AIController && AIController->bCanSeePlayer;
	
	// Time the AI will tolerate losing sight of the player.
	constexpr float LostSightGracePeriod = 1.5f;

	if (Data.bCanSeePlayer)
	{
		// The player is visible, so reset the lost-sight timer.
		Data.TimeSincePlayerSeen = 0.0f;
		Data.bShouldReturnToPatrol = false;
	}
	else
	{
		// The player is not visible, so accumulate elapsed time.
		Data.TimeSincePlayerSeen += DeltaTime;

		// Return to Patrol only after 1.5 seconds without sight.
		Data.bShouldReturnToPatrol = Data.TimeSincePlayerSeen >= LostSightGracePeriod;
	}
}
