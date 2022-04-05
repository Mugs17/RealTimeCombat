// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "AIAnimInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)

struct FAIAnimInstanceProxy : public FAnimInstanceProxy

{

    GENERATED_BODY()

        FAIAnimInstanceProxy()

        : FAnimInstanceProxy()

    {}

    FAIAnimInstanceProxy(UAnimInstance* Instance);

    virtual void Update(float DeltaSeconds) override;


public:

    AActor* OwningActor;

    //setting default value so its not null before being set by timer
    UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "Rotation Offsets")
        float HorizontalHeadRotation = 180;

    UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "Rotation Offsets")
        float VerticalUpperBodyRotation = 180;


};


UCLASS(Transient, Blueprintable)
class REALTIMECOMBAT_API UAIAnimInstance : public UAnimInstance
{
    GENERATED_BODY()


private:

    // The AllowPrivateAccess meta flag will allow this to be exposed to Blueprint,

    // but only to graphs internal to this class.

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Example", meta = (AllowPrivateAccess = "true"))

        FAIAnimInstanceProxy Proxy;

    virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override

    {

        // override this to just return the proxy on this instance

        return &Proxy;

    }

    virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override

    {

    }

    friend struct FAIAnimInstanceProxy;

    class AMyAIController* MyAIController;

    AActor* OwningActor;

public:

    virtual void NativeBeginPlay();

    void AnimSetClosestAllyActor();

    FRotator GetFaceDirectionDelta(AActor* TargetActor);
};
