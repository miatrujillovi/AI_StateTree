// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AIAgentController.generated.h"

class UStateTree;
class UStateTreeAIComponent;

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	bool bCanSeePlayer = false;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UFUNCTION()
	void BeginPlay() override;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UStateTreeAIComponent> StateTreeComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<AActor> Player;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
};
