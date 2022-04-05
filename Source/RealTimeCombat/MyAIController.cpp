// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AICharacter.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/PoseableMeshComponent.h"


AMyAIController::AMyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	//Creating sight config to hold config variables
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	//Setting config variables
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	//Setting what AI can detect
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;


	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AMyAIController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);



}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle CheckingKnownActorsHandle;
	GetWorldTimerManager().SetTimer(CheckingKnownActorsHandle, this, &AMyAIController::AIAttackEnemyWithinMeleeDistance, .4f, true, 1.f);

	int RandNumber = FMath::RandRange(0, 5);

	if (RandNumber >= 4)
	{
		bRandBoolForMovementTesting = false;
	}
}

void AMyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}


void AMyAIController::OnPossess(APawn* Pawn2)
{
	Super::OnPossess(Pawn2);

	//Sets OwningController on Possess because if set on begin play in the AICharacter, it could be null
	OwningAICharacter = Cast<AAICharacter>(Pawn2);
	OwningAICharacter->OwningController = this;

	SetGenericTeamId(FGenericTeamId(OwningAICharacter->TeamNumber));

}

FRotator AMyAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

//just a dynamic delegate attached to OnPerceptionUpdated
void AMyAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{

}

void AMyAIController::StartAIAttack()
{

	if (OwningAICharacter->ClosestEnemyCharacter)
	{
		MoveToActor(OwningAICharacter->ClosestEnemyCharacter, OwningAICharacter->AIDistanceToAttack);

		SetFocus(OwningAICharacter->ClosestEnemyCharacter, EAIFocusPriority::Gameplay);
		OwningAICharacter->bUseControllerRotationYaw = true;


		OwningAICharacter->StartAttack(OwningAICharacter->AIAttackType);
	}
}

void AMyAIController::AIAttackEnemyWithinMeleeDistance()
{
	TArray<AActor*> CurrentlyKnownEnemyActors;
	//GetPerceptionComponent()->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), CurrentlyKnownActors);
	GetPerceptionComponent()->GetHostileActors(CurrentlyKnownEnemyActors);
	//saving distance to closestActor
	float DistanceToClosestCharacter = 100000;

	if (CurrentlyKnownEnemyActors.Num() == 0)
	{
		//clearing closest enemy if no known enemy actors Note: Probably don't need both. Look into deleting variable on owning AI character
		OwningAICharacter->ClosestEnemyCharacter = nullptr;
		ClosestEnemyActor = nullptr;

		//Setting Closest Actor to nothing bc no actors are being sensed

		OwningAICharacter->ClosestEnemyCharacter = nullptr;
		ClearFocus(EAIFocusPriority::Move);

		MoveToActor(OwningAICharacter->NextWaypoint, 30.f, false, true, false);

		//set mode to walking
		OwningAICharacter->ChangeMovementSpeed(bRandBoolForMovementTesting);

		OwningAICharacter->bUseControllerRotationYaw = false;

		TArray<AActor*> CurrentlyKnownFriendlyActors;

		GetPerceptionComponent()->GetFilteredActors([](const FActorPerceptionInfo& ActorPerceptionInfo)
			{
				return !ActorPerceptionInfo.bIsHostile && ActorPerceptionInfo.HasAnyCurrentStimulus();
			}, CurrentlyKnownFriendlyActors);

		//UE_LOG(LogTemp, Warning, TEXT("Number of known friendly actors: %d, Team Number: %d"), CurrentlyKnownFriendlyActors.Num(), OwningAICharacter->TeamNumber);
		if (CurrentlyKnownFriendlyActors.Num() != 0)
		{
			SetClosestAlly(CurrentlyKnownFriendlyActors, 100000);
		}
		else if (CurrentlyKnownFriendlyActors.Num() == 0)
		{
			//Clearing closest ally if their is no known friendly actors
			ClosestAllyActor = nullptr;
		}
	}
	else
	{
		//Clearing closest ally actor if their is a known enemy
		ClosestAllyActor = nullptr;

		SetClosestEnemyAI(CurrentlyKnownEnemyActors, DistanceToClosestCharacter);

		//Checks to see if within attack Distance
		if (DistanceToClosestCharacter <= OwningAICharacter->AIDistanceToAttack + 100 && bCanAIAttack)
		{
			StartAIAttack();

			bCanAIAttack = false;
			GEngine->AddOnScreenDebugMessage(20, 2, FColor::Red, TEXT("Attack set to false"));

			//set timer to re-enable attacking after a certain period
			FTimerHandle AIAttackTimerHandle;

			float AttackDelay = FMath::RandRange(AttackRate * .8, AttackRate * 1.2);

			GetWorldTimerManager().SetTimer(AIAttackTimerHandle, this, &AMyAIController::TurnAttackOn, 3.f, false, AttackDelay);
		}
		//If not in attack distance move towards
		else
		{
			MoveToActor(OwningAICharacter->ClosestEnemyCharacter, OwningAICharacter->AIDistanceToAttack);
			SetFocus(OwningAICharacter->ClosestEnemyCharacter, EAIFocusPriority::Gameplay);
		}

	}
}

void AMyAIController::SetClosestEnemyAI(TArray<AActor*> CurrentlyKnownActors, float& DistanceToClosestCharacter)
{
	for (int i = 0; i < CurrentlyKnownActors.Num(); i++)
	{

		AActor* CurrentActor = CurrentlyKnownActors[i];
		//	UE_LOG(LogTemp, Error, TEXT("Distance To previous ClosestCharacter --- : %f Dustabce To Current: %f"), DistanceToClosestCharacter, OwningAICharacter->GetDistanceTo(CurrentActor));
		if (DistanceToClosestCharacter >= OwningAICharacter->GetDistanceTo(CurrentActor))
		{
			//	UE_LOG(LogTemp, Warning, TEXT("Distance to actor in array = %f "), DistanceToClosestCharacter);
			OwningAICharacter->ClosestEnemyCharacter = CurrentActor;
			ClosestEnemyActor = CurrentActor;
			DistanceToClosestCharacter = OwningAICharacter->GetDistanceTo(CurrentActor);
		}

	}
}

void AMyAIController::SetClosestAlly(TArray<AActor*> CurrentlyKnownActors, float DistanceToClosestCharacter)
{
	for (int i = 0; i < CurrentlyKnownActors.Num(); i++)
	{

		AActor* CurrentActor = CurrentlyKnownActors[i];

		float DistanceToCurrentActor = OwningAICharacter->GetDistanceTo(CurrentActor);
		//	UE_LOG(LogTemp, Error, TEXT("Distance To previous ClosestCharacter --- : %f Dustabce To Current: %f"), DistanceToClosestCharacter, OwningAICharacter->GetDistanceTo(CurrentActor));
		if (DistanceToClosestCharacter >= DistanceToCurrentActor)
		{
			//	UE_LOG(LogTemp, Warning, TEXT("Distance to actor in array = %f "), DistanceToClosestCharacter);
			ClosestAllyActor = CurrentActor;
			DistanceToClosestCharacter = DistanceToCurrentActor;
		}

	}
}

ETeamAttitude::Type AMyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
		}
	}

	return ETeamAttitude::Neutral;
}


void AMyAIController::TurnAttackOn()
{
	bCanAIAttack = true;

}

