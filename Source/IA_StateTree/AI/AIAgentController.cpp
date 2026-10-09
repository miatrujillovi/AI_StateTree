// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIAgentController.h"
#include "Components/StateTreeAIComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AAIAgentController::AAIAgentController()
{
	StateTreeComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAIComponent"));
	
	// We want to provide the parameters before the tree starts.
	StateTreeComponent->SetStartLogicAutomatically(false);
}

void AAIAgentController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	UE_LOG(LogTemp, Warning, TEXT("=== AI CONTROLLER POSSESSED ==="));

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		if (!Player)
		{
			UE_LOG(LogTemp, Error, TEXT("PLAYER NOT FOUND!"));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Player found: %s"), *Player->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Controller class: %s"), *GetClass()->GetName());

		if (!StateTreeComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("STATE TREE COMPONENT IS NULL!"));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("STATE TREE COMPONENT EXISTS!"));

		StateTreeComponent->StartLogic();

		UE_LOG(LogTemp, Warning, TEXT("StartLogic called"));
	});
}
