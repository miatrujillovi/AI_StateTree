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
	// Crea el componente que ejecuta el StateTree.
	StateTreeAIComponent = CreateDefaultSubobject<UStateTreeAIComponent>("StateTreeAIComponent");

	// Crea el componente de percepcion y la configuracion de vista.
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");

	// Que tan lejos ve, a que distancia lo pierde y el angulo del cono de vision.
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 2000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 60.0f;

	// Detecta a cualquier actor, sin importar su equipo.
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Registra el sentido de vista y usa este componente como la percepcion del controller.
	AIPerception->ConfigureSense(*SightConfig);
	SetPerceptionComponent(*AIPerception);
}

void AAIAgentController::BeginPlay()
{
	Super::BeginPlay();

	// Escucha el evento de percepcion: se dispara cuando el jugador entra o sale de la vista.
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AAIAgentController::HandleTargetPerceptionUpdated);
}

void AAIAgentController::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// Solo nos interesa el jugador; cualquier otro actor se ignora.
	const APawn* PerceivedPawn = Cast<APawn>(Actor);
	if (!PerceivedPawn || !PerceivedPawn->IsPlayerControlled())
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		// Lo ve: cancela cualquier "perdida" pendiente, guarda el objetivo y marca que lo tiene.
		GetWorldTimerManager().ClearTimer(LoseSightTimer);
		TargetActor = Actor;
		bHasTarget = true;
	}
	else if (bHasTarget && !GetWorldTimerManager().IsTimerActive(LoseSightTimer))
	{
		// Lo dejo de ver: espera LoseSightGraceTime antes de darlo por perdido,
		GetWorldTimerManager().SetTimer(LoseSightTimer, this, &AAIAgentController::ClearPlayerSight, LoseSightGraceTime, false);
	}
}

void AAIAgentController::ClearPlayerSight()
{
	// Paso el tiempo de gracia sin volver a verlo: ahora si se pierde.
	bHasTarget = false;
}
