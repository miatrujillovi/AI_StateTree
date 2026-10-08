// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "STE_CheckPlayerDistance.generated.h"

USTRUCT(BlueprintType)
struct IA_STATETREE_API FSTE_CheckPlayerDistanceInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Player")
	TObjectPtr<AActor> Player = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	float DistanceToPlayer = 0.f;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Check Player Distance", Category = "AI"))
struct IA_STATETREE_API FSTE_CheckPlayerDistance : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FSTE_CheckPlayerDistanceInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}
	
	virtual void TreeStart(FStateTreeExecutionContext& Context) const override;
	
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	
	virtual void TreeStop(FStateTreeExecutionContext& Context) const override;
};