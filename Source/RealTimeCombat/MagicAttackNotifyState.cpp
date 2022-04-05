// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicAttackNotifyState.h"
#include "BaseCharacter.h"
#include "Engine.h"


void UMagicAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Red, __FUNCTION__);

	if (MeshComp && MeshComp->GetOwner()) {
		Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character && Character->ClosestEnemyCharacter)
		{
			Character->StartMagicAttack(Character->ClosestEnemyCharacter);
		}
		else if (!Character->ClosestEnemyCharacter)
		{
			Character->StartMagicAttack(nullptr);
		}

	}
}

void UMagicAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Red, __FUNCTION__);

	if (MeshComp && MeshComp->GetOwner()) {
		Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			//Character->EndCollisionAttack();
		}
	}
}
