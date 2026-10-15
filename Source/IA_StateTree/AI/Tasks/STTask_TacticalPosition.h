// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "STTask_TacticalPosition.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

USTRUCT()
struct IA_STATETREE_API FSTTask_TacticalPositionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Tactical")
	TObjectPtr<UEnvQuery> TacticalQuery = nullptr;

	UPROPERTY()
	TObjectPtr<UEnvQueryInstanceBlueprintWrapper> QueryInstance = nullptr;

	UPROPERTY(EditAnywhere, Category = "Tactical")
	float AcceptanceRadius = 100.0f;

	UPROPERTY()
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY()
	bool bHasTargetLocation = false;
};

USTRUCT(meta = (DisplayName = "Find Tactical Position"))
struct FSTTask_TacticalPosition : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTTask_TacticalPositionInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};