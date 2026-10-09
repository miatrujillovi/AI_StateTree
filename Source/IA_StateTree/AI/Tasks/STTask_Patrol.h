// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/StateTreeAITask.h"
#include "STTask_Patrol.generated.h"

USTRUCT(BlueprintType)
struct IA_STATETREE_API FSTTask_PatrolInstanceData
{
	GENERATED_BODY()
	
	//Contexto del AI Agent Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<AAIAgentCharacter> ActorContext = nullptr;
	
	//Output de destino de la IA
	UPROPERTY(EditAnywhere, Category = "AI")
	FVector TargetLocation = FVector::ZeroVector;
	
	//Estado interno para que el StateTree recuerde por donde va
	UPROPERTY()
	int32 CurrentIndex = 0;
	
	/*UPROPERTY(EditAnywhere, Category = "Patrol")
	TObjectPtr<AActor> PatrolPointA;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	TObjectPtr<AActor> PatrolPointB;*/

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float AcceptanceRadius = 100.f;
	
	UPROPERTY()
	bool bGoingToA = true;
};

USTRUCT(meta = (DisplayName = "Get Patrolling Points"))
struct IA_STATETREE_API FSTTask_PatrolPoints : public FStateTreeTaskBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FSTTask_PatrolInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct();}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Patrol", Category = "AI"))
struct IA_STATETREE_API FSTTask_Patrol : public FStateTreeAITaskBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FSTTask_PatrolInstanceData;
	
	FSTTask_Patrol() = default;
	
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	//STATES
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};