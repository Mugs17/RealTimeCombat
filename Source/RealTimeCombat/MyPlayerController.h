// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class REALTIMECOMBAT_API AMyPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMyPlayerController();

protected:

	virtual void OnPossess(APawn* aPawn) override;


private:

	// Implement The Generic Team Interface 
	FGenericTeamId TeamId = 0;

	FGenericTeamId GetGenericTeamId() const;
	
};
