// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIAgentCharacter.generated.h"

USTRUCT(BlueprintType)
struct FPatrolPoints
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeEditWidget))
	FVector Location;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitDuration = 0.0f;
};

USTRUCT(BlueprintType)
struct FProperties
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPatrolPoints> PatrolPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentPatrolPointIndex = 0;
};

UCLASS()
class IA_STATETREE_API AAIAgentCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIAgentCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FProperties PatrolPoints;
	
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<AActor> PatrolPointA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<AActor> PatrolPointB;*/

public:	
	/*AActor* GetPatrolPointA() const
	{
		return PatrolPointA;
	}

	AActor* GetPatrolPointB() const
	{
		return PatrolPointB;
	}*/

};
