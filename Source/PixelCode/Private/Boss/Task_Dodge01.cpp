// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_Dodge01.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossApernia.h"

UTask_Dodge01::UTask_Dodge01(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Dodge Right");

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AS_Dodge01_Montage.AS_Dodge01_Montage'"));
    if (montageObj.Succeeded())
    {
        dodge01 = montageObj.Object;
    }
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_Dodge01::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);

    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
    player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);



    if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
    {
        bossController->StopMovement();
        APawn* ControlledPawn = bossController->GetPawn();
        if (ControlledPawn)
        {
            ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

            if (dodge01 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance())
            {
                //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                boss->ServerRPC_DodgeRight();

                // LaunchCharacter를 호출
                FVector LaunchVelocity(0, 800, 0);

                boss->LaunchCharacter(LaunchVelocity, true, true);
                

                
                if (player)
                {
                    //플레이어의 위치를 얻어낸다
                    playerLocation = player->GetActorLocation();
                    //보스컨트롤러를 캐스팅
                    bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
                    if (bossController)
                    {
                        APawn* bossPawn = bossController->GetPawn();
                        if (bossPawn)
                        {
                            // 방향 설정
                            FVector direction = playerLocation - bossPawn->GetActorLocation();
                            direction.Z = 0; // 보스가 수평으로만 회전하도록 Z축 회전 제거
                            FRotator newRotation = direction.Rotation();
                            bossPawn->SetActorRotation(newRotation);

                        }
                    }
                }

            }
        }
    }
    

    return EBTNodeResult::InProgress;
}

void UTask_Dodge01::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 태스크 시작 시 초기화
    if (currentTime == 0.0f)
    {
        animOnce = false;
    }

    currentTime += DeltaSeconds;

    // 1.8초가 지나면 태스크 완료
    if (currentTime >= 0.5f)
    {
        currentTime = 0.0f; // currentTime 초기화
        dodgeRight = false;
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        BlackboardComp = OwnerComp.GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsBool(dodgeRightCoolTime.SelectedKeyName, dodgeRight);
        }
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        
        
    }
}
