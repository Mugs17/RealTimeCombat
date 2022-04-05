// Fill out your copyright notice in the Description page of Project Settings.


#include "Waypoint.h"
#include "Components/BoxComponent.h"
#include "AICharacter.h"

// Sets default values
AWaypoint::AWaypoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Waypoint Collision Box");
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWaypoint::OnAIBeginOverlap);
	BoxComponent->bHiddenInGame = false;

}

// Called when the game starts or when spawned
void AWaypoint::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaypoint::OnAIBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AAICharacter* OverlappedAI = Cast<AAICharacter>(OtherActor))
	{
		if (!OverlappedAI->ClosestEnemyCharacter)
		{
			OverlappedAI->NextWaypoint = NextWaypoint;
			OverlappedAI->MoveToLocation(NextWaypoint->GetActorLocation());
		}

	}
}

