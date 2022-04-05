// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackNotifyState.h"
#include "BaseCharacter.h"
#include "Engine.h"

void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Red, __FUNCTION__);

	if (MeshComp && MeshComp->GetOwner()) {
		Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->StartCollisionAttack();
		}
	}


}

void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Red, __FUNCTION__);

	if (MeshComp && MeshComp->GetOwner()) {
		Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->EndCollisionAttack();
		}
	}
}
