// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_BossFirstEquitSword.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_BossFirstEquitSword : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
    UTask_BossFirstEquitSword(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    //virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    float currentTime;
    float escapeTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    class UAnimMontage* swordEquitMT;

    bool bSwordEquit = false;
};
