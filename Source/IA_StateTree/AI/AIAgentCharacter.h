// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIAgentCharacter.generated.h"

UCLASS()
class IA_STATETREE_API AAIAgentCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIAgentCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<AActor> PatrolPointA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<AActor> PatrolPointB;

public:	
	AActor* GetPatrolPointA() const
	{
		return PatrolPointA;
	}

	AActor* GetPatrolPointB() const
	{
		return PatrolPointB;
	}

};
