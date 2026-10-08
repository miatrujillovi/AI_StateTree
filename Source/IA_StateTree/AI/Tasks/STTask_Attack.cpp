// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/STTask_Attack.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"


EStateTreeRunStatus FSTTask_Attack::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FSTTask_AttackInstanceData& Data = Context.GetInstanceData(*this);
	
	Data.ElapsedTime = 0.f;
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_Attack::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FSTTask_AttackInstanceData& Data = Context.GetInstanceData(*this);
	
	Data.ElapsedTime += DeltaTime;
	
	if (Data.ElapsedTime > Data.AttackDuration)
	{
		return EStateTreeRunStatus::Succeeded;
	}
	
	return EStateTreeRunStatus::Running;
}

void FSTTask_Attack::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	//return EStateTreeRunStatus::Succeeded;
}
