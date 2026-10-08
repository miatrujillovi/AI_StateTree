// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIAgentController.generated.h"

class UStateTree;
class UStateTreeAIComponent;

UCLASS()
class IA_STATETREE_API AAIAgentController : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIAgentController();
	
	AActor* GetPlayer() const
	{
		return Player;
	}
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UStateTreeAIComponent> StateTreeComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UStateTree> AssignedStateTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<AActor> Player;
};
