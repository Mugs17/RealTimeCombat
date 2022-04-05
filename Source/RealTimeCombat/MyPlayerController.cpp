// Fill out your copyright notice in the Description page of Project Settings.



#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	SetGenericTeamId(TeamId);
}

FGenericTeamId AMyPlayerController::GetGenericTeamId() const
{
	return TeamId;
}
