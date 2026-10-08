// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/StateTreeAITask.h"
#include "STTask_ChasePlayer.generated.h"

USTRUCT(BlueprintType)
struct IA_STATETREE_API FSTTask_ChasePlayerInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Chase")
	TObjectPtr<AActor> Player;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float AcceptanceRadius = 150.f;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Chase Player", Category = "AI"))
struct IA_STATETREE_API FSTTask_ChasePlayer : public FStateTreeAITaskBase
{
	GENERATED_BODY()
	
	FSTTask_ChasePlayer() = default;
	
	using FInstanceDataType = FSTTask_ChasePlayerInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}
	
	//STATES
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
