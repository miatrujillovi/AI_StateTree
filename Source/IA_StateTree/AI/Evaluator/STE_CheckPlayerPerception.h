// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "STE_CheckPlayerPerception.generated.h"

USTRUCT(BlueprintType)
struct IA_STATETREE_API FSTE_CheckPlayerPerceptionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Player")
	TObjectPtr<AActor> Player = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Perception")
	bool bCanSeePlayer = false;
	
	UPROPERTY(EditAnywhere, Category = "Perception")
	float TimeSincePlayerSeen = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Perception")
	bool bShouldReturnToPatrol = false;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Check Player Perception", Category = "AI"))
struct IA_STATETREE_API FSTE_CheckPlayerPerception : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FSTE_CheckPlayerPerceptionInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}
	
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
