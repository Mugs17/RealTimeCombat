// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class REALTIMECOMBAT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()



public:
	ABaseCharacter();

	AActor* ClosestEnemyCharacter;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseLookUpRate;


	UFUNCTION()
		void OnAttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
		float AttackImpulseAmount = 500;

	void StartCollisionAttack();

	void StartMagicAttack(AActor* EnemyActor);

	void EndCollisionAttack();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		class USoundCue* PunchSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		class UAudioComponent* PunchAudioComponent;


	void StartAttack(FString AttackType);

	//Set attack variable boolean for which attack options to use and then call Start Attack to set attack in motion
	void SetAttackSlot_1();

	FString AttackSlot_1 = TEXT("Melee");

	void SetAttackSlot_2();

	FString AttackSlot_2 = TEXT("Magic");

	void AddImpulseLerp(APawn* InstigatorCharacter, APawn* OtherCharacter);


	bool bIsCharacterEffectedByImpulse = false;

	FVector ImpulseStartLocation;
	FVector ImpulseEndLocation;
	float ImpulseTimeElapsed;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float ImpulseTotalDuration;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float ImpulseTravelDistance;

	ABaseCharacter* ImpulsedCharacter;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		class UAnimMontage* MeleeAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		class UAnimMontage* MagicAttackMontage;

	//box compoenent for melee fist collisions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
		class UBoxComponent* LeftFistCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
		class UBoxComponent* RightFistCollisionBox;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
		FName CharacterWeaponTeamCollisonName;


	FName CharacterMovementCollisionName;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		TSubclassOf<class AMagicBallActor> MagicBallClass;

	UAnimInstance* AnimInstance;

};
