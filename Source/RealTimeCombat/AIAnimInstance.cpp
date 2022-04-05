// Fill out your copyright notice in the Description page of Project Settings.

#include "AIAnimInstance.h"
//#include "MyAIController.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

void UAIAnimInstance::NativeBeginPlay()
{


	//MyAIController = Cast<AMyAIController>(GetOwningActor()->GetInstigatorController());

	FTimerHandle SetClosestAllyTimerHandle;

	OwningActor = GetOwningActor();

	Proxy.OwningActor = OwningActor;


	//MyAIController->GetWorldTimerManager().SetTimer(SetClosestAllyTimerHandle, this, &UAIAnimInstance::AnimSetClosestAllyActor, .05, true, 1);
}

void UAIAnimInstance::AnimSetClosestAllyActor()
{
	/*
	if (MyAIController)
	{
		//Resetting rotations so that actor will return to baseline if rotation conditions arent met
		Proxy.VerticalUpperBodyRotation = 0;
		Proxy.HorizontalHeadRotation = 180;

		if (MyAIController->ClosestAllyActor && GetOwningActor()->GetDistanceTo(MyAIController->ClosestAllyActor) <= 2000)
		{

			FRotator FaceDirectionDelta = GetFaceDirectionDelta(MyAIController->ClosestAllyActor);

			Proxy.HorizontalHeadRotation = FaceDirectionDelta.Yaw;


			UE_LOG(LogTemp, Warning, TEXT("Proxy horizontal head rotation +++++ = : %f"), Proxy.HorizontalHeadRotation);

		}
		else if (MyAIController->ClosestEnemyActor)
		{
			FRotator FaceDirectionDelta = GetFaceDirectionDelta(MyAIController->ClosestEnemyActor);

			Proxy.VerticalUpperBodyRotation = FaceDirectionDelta.Pitch * 3;
			UE_LOG(LogTemp, Warning, TEXT("Face Direction Pitch being set: %f"), Proxy.VerticalUpperBodyRotation);
		}
	}
	*/
}

FRotator UAIAnimInstance::GetFaceDirectionDelta(AActor* TargetActor)
{
	FVector OwningActorLocation;

	FVector AllyActorLocation;

	OwningActorLocation = OwningActor->GetActorLocation();

	AllyActorLocation = TargetActor->GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwningActorLocation, AllyActorLocation);

	FRotator OwningActorCurrentRotation = OwningActor->GetActorRotation();

	FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(OwningActorCurrentRotation, LookAtRotation);

	return DeltaRotator;
}

void FAIAnimInstanceProxy::Update(float DeltaSeconds)
{

	{

		// Update internal variables


	}
}
