// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class REALTIMECOMBAT_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()

private:
	AAICharacter();

	virtual void BeginPlay() override;



public:

	UPROPERTY(EditAnywhere, Category = "Team")
		uint8 TeamNumber;

	UPROPERTY()
		class AMyAIController* OwningController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		AActor* NextWaypoint;

	void StartAIMovement();

	//Allows other classes to move actor without needing to cast to the actors AI controller
	void MoveToLocation(FVector Location);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
		float AIDistanceToAttack = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
		FString AIAttackType = "Melee";

	FString LineTraceSide = "Left";

	void CallAvoidance();

	void CreateObstacleAvoidanceLineTrace(FString Direction);

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float AvoidanceLineTraceLength = 150;

	void ChangeMovementSpeed(bool bIsWalking);
	
};
