// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicBallActor.generated.h"

UCLASS()
class REALTIMECOMBAT_API AMagicBallActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMagicBallActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnFireballBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bHasOwnerBeenSet = false;


	//Gets called from StartAttack
	void SetMagicBallHoming(AActor* TargetActor);

	//Gets called from StartAttack
	void SetMagicBallAimRotation(AActor* TargetActor);


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		class USoundCue* MagicBallSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		class UAudioComponent* MagicBallAudioComponent;


private:

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UStaticMeshComponent* FireballInvisibleMesh;


	UPROPERTY(VisibleAnywhere, Category = "Movement")
		class UProjectileMovementComponent* MagicBallMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
		class UNiagaraComponent* FlyingFireBallParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
		class UNiagaraSystem* MagicBallDeathExplosion;



	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		float FireBallDamageAmount = 20;

	void DestroyMagicBall();

};
