// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIAgentController.h"
#include "Components/StateTreeAIComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"

AAIAgentController::AAIAgentController()
{
	StateTreeComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAIComponent"));
	
	// We want to provide the parameters before the tree starts.
	StateTreeComponent->SetStartLogicAutomatically(false);
	
	//Configure AI Perception - Sight Perception
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	
	SightConfig->SightRadius = 1500.f; //Maximum detection range
	SightConfig->LoseSightRadius = 1800.f; //Larger range for retaining an existing target
	SightConfig->PeripheralVisionAngleDegrees = 60.f; //Field of View FOV
	
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	
	SightConfig->SetMaxAge(3.f);
	
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
}

void AAIAgentController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	//Only react to the player.
	if (!Actor || Actor != Player)
	{
		return;
	}
	
	const bool bNewCanSeePlayer = Stimulus.WasSuccessfullySensed();
	
	//Ignore repeated notifications that don't change visibility.
	if (bCanSeePlayer == bNewCanSeePlayer)
	{
		return;
	}
	
	bCanSeePlayer = bNewCanSeePlayer;
	
	UE_LOG(LogTemp, Warning, TEXT("PLAYER SIGHT CHANGED: %s"), bCanSeePlayer ? TEXT("VISIBLE") : TEXT("NOT VISIBLE"));
}

void AAIAgentController::BeginPlay()
{
	Super::BeginPlay();
	
	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIAgentController::OnTargetPerceptionUpdated);
	}
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
