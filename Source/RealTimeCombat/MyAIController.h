// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class REALTIMECOMBAT_API AMyAIController : public AAIController
{
	GENERATED_BODY()

		AMyAIController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* Pawn) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;

public:

	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AISightRadius = 1000.0f;
	//
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AISightAge = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AILoseSightRadius = AISightRadius + 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AIFieldOfView = 90.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY()
		class AAICharacter* OwningAICharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		AActor* ClosestActor;

	AActor* ClosestAllyActor;

	AActor* ClosestEnemyActor;


	void StartAIAttack();

	void AIAttackEnemyWithinMeleeDistance();

	void SetClosestEnemyAI(TArray<AActor*> CurrentlyKnownActors, float& DistanceToClosestCharacter);

	void SetClosestAlly(TArray<AActor*> CurrentlyKnownActors, float DistanceToClosestCharacter);


	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;


private:

	bool bCanAIAttack = true;

	float AttackRate = 2.f;

	void TurnAttackOn();

	bool bRandBoolForMovementTesting = true;


	
};
