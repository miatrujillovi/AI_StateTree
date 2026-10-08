// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/StateTreeAITask.h"
#include "STTask_Attack.generated.h"

USTRUCT(BlueprintType)
struct IA_STATETREE_API FSTTask_AttackInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float AttackDuration = 2.f;

	float ElapsedTime = 0.f;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Attack", Category = "AI"))
struct IA_STATETREE_API FSTTask_Attack : public FStateTreeAITaskBase
{
	GENERATED_BODY()
	
	FSTTask_Attack() = default;
	
	using FInstanceDataType = FSTTask_AttackInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}
	
	//STATES
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
