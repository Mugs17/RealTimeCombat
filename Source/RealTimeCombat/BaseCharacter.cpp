// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MagicBallActor.h"
#include "AIAnimInstance.h"


ABaseCharacter::ABaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//Setting up Animation Montage Objects so they can be referenced / used
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeAttackMontageObject(TEXT("AnimMontage'/Game/TUTORIAL_RESOURCES/Animations/MeleeAttackMontage.MeleeAttackMontage'"));

	if (MeleeAttackMontageObject.Succeeded())
	{
		MeleeAttackMontage = MeleeAttackMontageObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MagicAttackMontageObject(TEXT("AnimMontage'/Game/TUTORIAL_RESOURCES/Animations/MagicAttackMontage.MagicAttackMontage'"));

	if (MagicAttackMontageObject.Succeeded())
	{
		MagicAttackMontage = MagicAttackMontageObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> PunchSoundCueObject(TEXT("SoundCue'/Game/Blueprints/MeleeAttackSoundCue.MeleeAttackSoundCue'"));

	if (PunchSoundCueObject.Succeeded())
	{
		PunchSoundCue = PunchSoundCueObject.Object;

		PunchAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Punch Audio Component"));
		PunchAudioComponent->SetupAttachment(RootComponent);
	}


	LeftFistCollisionBox = CreateDefaultSubobject<UBoxComponent>("Left Fist Collision Box");
	LeftFistCollisionBox->SetupAttachment(RootComponent);
	LeftFistCollisionBox->SetCollisionProfileName("NoCollision");
	LeftFistCollisionBox->SetVisibility(true);
	LeftFistCollisionBox->bHiddenInGame = false;

	RightFistCollisionBox = CreateDefaultSubobject<UBoxComponent>("Right Fist Collision Box");
	RightFistCollisionBox->SetupAttachment(RootComponent);
	RightFistCollisionBox->SetCollisionProfileName("NoCollision");
	RightFistCollisionBox->SetVisibility(true);
	RightFistCollisionBox->bHiddenInGame = false;

	AnimInstance = GetMesh()->GetAnimInstance();

	UAIAnimInstance* AIAnimInstance = Cast<UAIAnimInstance>(AnimInstance);

}

//////////////////////////////////////////////////////////////////////////
// Input



void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterWeaponTeamCollisonName == "EnemyWeapon")
	{
		CharacterMovementCollisionName == "EnemyMovement";
	}
	else if (CharacterWeaponTeamCollisonName == "AllyWeapon")
	{
		CharacterMovementCollisionName == "AllyMovement";
	}

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	LeftFistCollisionBox->AttachToComponent(GetMesh(), TransformRules, TEXT("hand_l_collision"));
	LeftFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnAttackHit);


	RightFistCollisionBox->AttachToComponent(GetMesh(), TransformRules, TEXT("hand_r_collision"));
	RightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnAttackHit);

	if (PunchAudioComponent)
	{
		PunchAudioComponent->SetSound(PunchSoundCue);
	}

}

void ABaseCharacter::Tick(float DeltaSeconds)
{

	if (bIsCharacterEffectedByImpulse)
	{
		if (ImpulseTimeElapsed < ImpulseTotalDuration)
		{
			ImpulsedCharacter->SetActorLocation(FMath::Lerp(ImpulseStartLocation, ImpulseEndLocation, ImpulseTimeElapsed / ImpulseTotalDuration));
			ImpulseTimeElapsed += DeltaSeconds;
		}
		else if (ImpulseTimeElapsed > ImpulseTotalDuration)
		{
			bIsCharacterEffectedByImpulse = false;
			ImpulseTimeElapsed = 0;
		}

	}
}

void ABaseCharacter::StartAttack(FString AttackType)
{
	if (AttackType == "Melee")
	{
		int32 SectionNumber = rand() % 2 + 1;
		FString SectionName = "start_" + FString::FromInt(SectionNumber);

		PlayAnimMontage(MeleeAttackMontage, 1.f, FName(*SectionName));
	}
	else if (AttackType == "Magic")
	{
		int32 SectionNumber = 1;
		FString SectionName = "start_" + FString::FromInt(SectionNumber);

		PlayAnimMontage(MagicAttackMontage, 1.f, FName(*SectionName));
	}

}

void ABaseCharacter::SetAttackSlot_1()
{

	//call start attack
	StartAttack("Melee");
}

void ABaseCharacter::SetAttackSlot_2()
{
	//call start attack
	StartAttack("Magic");
}

void ABaseCharacter::AddImpulseLerp(APawn* InstigatorCharacter, APawn* OtherCharacter)
{

	ImpulseStartLocation = OtherCharacter->GetActorLocation();
	FVector InstigatorForwardVector = InstigatorCharacter->GetActorForwardVector();
	ImpulseEndLocation = (InstigatorForwardVector * ImpulseTravelDistance) + ImpulseStartLocation;


	ImpulsedCharacter = Cast<ABaseCharacter>(OtherCharacter);

	if (ImpulsedCharacter)
	{
		bIsCharacterEffectedByImpulse = true;
		ImpulseTimeElapsed = 0;
		//	UE_LOG(LogTemp, Error, TEXT("Add Impulse has started succesfully"));
	}

	//DeltaSeconds;
	//Character->SetActorLocation(FMath::Lerp(StartLocation, EndLocation, TimeElapsed / LerpDuration));
}


void ABaseCharacter::OnAttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//if colliding with self ignore
	//UE_LOG(LogTemp, Error, TEXT("Actor hit: %s, Component hit: %s"), *OtherActor->GetName(), *OtherComp->GetName());
	if (OtherActor != this)
	{
		if (PunchAudioComponent)
		{
			PunchAudioComponent->Play();
		}

		if (AnimInstance)
		{
			AnimInstance->Montage_JumpToSectionsEnd(AnimInstance->Montage_GetCurrentSection(), MeleeAttackMontage);
			AnimInstance->Montage_SetPlayRate(MeleeAttackMontage, 0.5f);
		}

		ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
		if (OtherCharacter)
		{
			FVector ThisForwardVector = GetActorForwardVector();
			FVector VectorWithImpulse = ThisForwardVector * AttackImpulseAmount;
			VectorWithImpulse = FVector(500, 500, 0);

			AddImpulseLerp(this, OtherCharacter);

			//Apply Damage

			UGameplayStatics::ApplyDamage(OtherCharacter, 30.0f, this->GetController(), this, UDamageType::StaticClass());

			ClosestEnemyCharacter = nullptr;

		}



	}


}

void ABaseCharacter::StartCollisionAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Team Collision Name %s"), *CharacterWeaponTeamCollisonName.ToString());
	LeftFistCollisionBox->SetCollisionProfileName(CharacterWeaponTeamCollisonName);
	LeftFistCollisionBox->SetNotifyRigidBodyCollision(true);

	RightFistCollisionBox->SetCollisionProfileName(CharacterWeaponTeamCollisonName);
	UE_LOG(LogTemp, Error, TEXT("Collision Profile Turned on: %s"), *CharacterWeaponTeamCollisonName.ToString());
	RightFistCollisionBox->SetNotifyRigidBodyCollision(true);

}

void ABaseCharacter::StartMagicAttack(AActor* EnemyActor)
{
	FVector MagicBallSpawnPoint = GetMesh()->GetSocketLocation("hand_r_projectile");
	//Setting the owner of the fireball so it can use that information to apply damage
	AMagicBallActor* FireBallActor = GetWorld()->SpawnActor<AMagicBallActor>(MagicBallClass, MagicBallSpawnPoint, this->GetActorRotation());
	FireBallActor->SetOwner(this);
	FireBallActor->bHasOwnerBeenSet = true;

	UE_LOG(LogTemp, Error, TEXT("Fireball Should be spawning--------------------------"));

	if (EnemyActor)
	{
		//FireBallActor->SetMagicBallHoming(EnemyActor); // Can home towards enemy if desired
		FireBallActor->SetMagicBallAimRotation(EnemyActor);
	}

}

void ABaseCharacter::EndCollisionAttack()
{
	LeftFistCollisionBox->SetCollisionProfileName("NoCollision");
	UE_LOG(LogTemp, Error, TEXT("Collision Profile Turned on: NoCollision"));
	LeftFistCollisionBox->SetNotifyRigidBodyCollision(false);

	RightFistCollisionBox->SetCollisionProfileName("NoCollision");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(false);


}