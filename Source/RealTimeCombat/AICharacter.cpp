// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyAIController.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

#include "Engine/Engine.h"


AAICharacter::AAICharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0, 200.f, 0);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	GetRootComponent();
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle StartAIMovementTimerHandle;

	GetWorldTimerManager().SetTimer(StartAIMovementTimerHandle, this, &AAICharacter::StartAIMovement, 1.f, false, 3.f);

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);


	FTimerHandle LineTraceMovementAvoidanceHandle;

	GetWorldTimerManager().SetTimer(LineTraceMovementAvoidanceHandle, this, &AAICharacter::CallAvoidance, .2f, true, .2f);


}

void AAICharacter::StartAIMovement()
{

	if (NextWaypoint != nullptr)
	{
		OwningController->MoveToActor(NextWaypoint, 5.f);
	}

}

void AAICharacter::MoveToLocation(FVector Location)
{
	if (OwningController)
	{
		OwningController->MoveToLocation(Location, 10.f);
	}

}

void AAICharacter::CallAvoidance()
{
	//Switching line trace so that their movement functionality doesnt interfere with eachother
	if (LineTraceSide == "Left")
	{
		LineTraceSide = "Right";
	}
	else if (LineTraceSide == "Right")
	{
		LineTraceSide = "Left";
	}
	CreateObstacleAvoidanceLineTrace(LineTraceSide);
}

void AAICharacter::CreateObstacleAvoidanceLineTrace(FString Direction)
{
	TArray<AActor*> ActorsToAvoid;
	ActorsToAvoid.Add(this);

	FString SocketName = "LineTraceSocket";

	FString TotalSocketName = Direction + "LineTraceSocket";

	FVector LineTraceStartLocation = GetMesh()->GetSocketLocation(*TotalSocketName);

	//UE_LOG(LogTemp, Warning, TEXT("Line trace socket %s"), *TotalSocketName);

	FVector LineTraceRotation = GetActorRotation().Vector();

	FVector LineTraceEnd = LineTraceStartLocation + LineTraceRotation * AvoidanceLineTraceLength;


	FHitResult LineTraceHitResult;


	ECollisionChannel LineTraceCollisionChannel;

	if (CharacterWeaponTeamCollisonName == "AllyWeapon")
	{
		//ally trace channel
		//UE_LOG(LogTemp, Warning, TEXT("Ally Collision Set ------------"));
		LineTraceCollisionChannel = ECollisionChannel::ECC_GameTraceChannel10;
	}
	else
	{
		//enemy trace channel
		//UE_LOG(LogTemp, Warning, TEXT("Enemy Collision Set ------------"));
		LineTraceCollisionChannel = ECollisionChannel::ECC_GameTraceChannel9;
	}

	bool bSuccessLeft = GetWorld()->LineTraceSingleByChannel(LineTraceHitResult, LineTraceStartLocation, LineTraceEnd, LineTraceCollisionChannel);

	if (bSuccessLeft && LineTraceHitResult.GetActor() != this)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Line trace should be working"));
		DrawDebugLine(GetWorld(), LineTraceStartLocation, LineTraceEnd, FColor::Blue, false, .2f, 2, 1.0f);

		//avoid hit actor
		if (Direction == "Left")
		{
			FVector RightSideLocation = GetActorLocation() + GetActorRightVector() * 200;

			FVector ForwardLocation = GetActorLocation() + GetActorForwardVector() * 200;

			MoveToLocation((ForwardLocation / 3) * 2 + RightSideLocation / 3);
		}
		else if (Direction == "Right")
		{
			FVector LeftSideLocation = GetActorLocation() + (GetActorRightVector() * -1) * 200;

			FVector ForwardLocation = GetActorLocation() + GetActorForwardVector() * 200;

			MoveToLocation((ForwardLocation / 3) * 2 + LeftSideLocation / 3);
		}

	}

	//UE_LOG(LogTemp, Warning, TEXT("Line trace function playing"));

}

void AAICharacter::ChangeMovementSpeed(bool bIsWalking)
{

	if (bIsWalking)
	{
		GetCharacterMovement()->MaxWalkSpeed = 100;
		AvoidanceLineTraceLength = 50;
	}
	else if (!bIsWalking)
	{
		GetCharacterMovement()->MaxWalkSpeed = 300;
		AvoidanceLineTraceLength = 150;
	}

}
