

#include "MagicBallActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "PlayerCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Waypoint.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMagicBallActor::AMagicBallActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireballInvisibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fireball Mesh"));
	FireballInvisibleMesh->SetVisibility(false);
	FireballInvisibleMesh->bHiddenInGame = true;

	FireballInvisibleMesh->SetRelativeRotation(FireballInvisibleMesh->GetRelativeRotation() * -1);

	RootComponent = FireballInvisibleMesh;;


	MagicBallMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	MagicBallMovementComponent->MaxSpeed = 2000.f;
	MagicBallMovementComponent->InitialSpeed = 600.f;
	MagicBallMovementComponent->ProjectileGravityScale = 0;



	MagicBallMovementComponent->bRotationRemainsVertical = false;



	FlyingFireBallParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Flying Fire Ball Particles"));
	FlyingFireBallParticles->SetupAttachment(RootComponent);


	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> MagicBallDeathParticleObject(TEXT("NiagaraSystem'/Game/Particles/FireExplosion_N.FireExplosion_N'"));

	if (MagicBallDeathParticleObject.Succeeded())
	{
		MagicBallDeathExplosion = MagicBallDeathParticleObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> MagicBallSoundCueObject(TEXT("SoundCue'/Game/Blueprints/FireballAttackSoundCue.FireballAttackSoundCue'"));

	if (MagicBallSoundCueObject.Succeeded())
	{
		MagicBallSoundCue = MagicBallSoundCueObject.Object;

		MagicBallAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Magic Ball Audio Component"));
		MagicBallAudioComponent->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AMagicBallActor::BeginPlay()
{
	Super::BeginPlay();



	FireballInvisibleMesh->OnComponentBeginOverlap.AddDynamic(this, &AMagicBallActor::OnFireballBeginOverlap);

	if (MagicBallAudioComponent)
	{
		MagicBallAudioComponent->SetSound(MagicBallSoundCue);
	}



}

void AMagicBallActor::OnFireballBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Overlap Working if nothing follows GetOWner() returning nullptr"));
	AActor* MyActor = GetOwner();

	//this should only happen if owner was destroyed.
	if (GetOwner() == nullptr && bHasOwnerBeenSet)
	{
		DestroyMagicBall();
		return;
	}
	//if owner hasnt been set I dont want to run any more checks and ignore this overlap
	if (GetOwner() == nullptr) return;
	//UE_LOG(LogTemp, Error, TEXT("Overlap Working if nothing follows GetOWner() returning nullptr"));UE_LOG(LogTemp, Error, TEXT("Overlap Working Owner exists"));
	if (OtherActor != this && OtherActor != MyActor /* && Cast<ACharacter>(OtherActor)*/)
	{
		//Apply Damage
		UGameplayStatics::ApplyDamage(OtherActor, FireBallDamageAmount, MyActor->GetInstigatorController(), this, UDamageType::StaticClass());
		UE_LOG(LogTemp, Error, TEXT("apply damage is being run"));

		//Destroy projectile
		DestroyMagicBall();
	}
	//Instead of casting I can create a custom collision profile for collisions. 
	else if (OtherActor != this && OtherActor != MyActor && !Cast<AWaypoint>(OtherActor) && !Cast<AMagicBallActor>(OtherActor))
	{
		DestroyMagicBall();
	}



}

// Called every frame
void AMagicBallActor::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);


}

void AMagicBallActor::SetMagicBallHoming(AActor* TargetActor)
{

	//if Actor isnt null, the cast will succeed so just checking the actor
	if (TargetActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Homing is being played"));
		MagicBallMovementComponent->bIsHomingProjectile = true;
		MagicBallMovementComponent->HomingAccelerationMagnitude = 700;

		USceneComponent* HitSceneActor = Cast<USceneComponent>(TargetActor->GetComponentByClass(USceneComponent::StaticClass()));
		MagicBallMovementComponent->HomingTargetComponent = HitSceneActor;
	}
}

void AMagicBallActor::SetMagicBallAimRotation(AActor* TargetActor)
{
	AActor* OwningActor = GetOwner();

	FRotator DeltaRotation;

	if (OwningActor)
	{
		FVector OwningActorLocation = OwningActor->GetActorLocation();

		FVector TargetActorLocation = TargetActor->GetActorLocation();

		FRotator TargetLookatRotation = UKismetMathLibrary::FindLookAtRotation(OwningActorLocation, TargetActorLocation);

		FRotator CurrentOwnerRotation = OwningActor->GetActorRotation();

		DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentOwnerRotation, TargetLookatRotation);

		MagicBallMovementComponent->Velocity = FVector(MagicBallMovementComponent->Velocity.X, MagicBallMovementComponent->Velocity.Y, DeltaRotation.Pitch * -10);


		//UE_LOG(LogTemp, Warning, TEXT("----------------------------------------------------------"));

	}
}

void AMagicBallActor::DestroyMagicBall()
{
	Destroy();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MagicBallDeathExplosion, GetActorLocation(), GetActorRotation());
	MagicBallAudioComponent->Play();
}

