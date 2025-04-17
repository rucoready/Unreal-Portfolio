# Unreal-Portfolio

포트폴리오 목표로 제작된 멀티 RPG 데모 게임입니다.<br/>
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 엔진 : Unreal Engine5 5.4 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 규모 : 3인 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 제작기간 : 4주 <br/>

# 사용한 개발 도구<br/>

&nbsp;&nbsp;&nbsp;&nbsp;● Microsoft Visual Studio <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Blender<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● GithubDesktop<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● PhotoShop<br/>

# 데모 영상 
| SinglePlay | MultiPlay |
| --- | --- |
| <a href="https://youtu.be/PPo4rxRjsWc"><img src="https://img.youtube.com/vi/PPo4rxRjsWc/0.jpg" width="300"> </a> | <br/><a href="https://youtu.be/9zh2KllxqSw"><img src="https://img.youtube.com/vi/9zh2KllxqSw/0.jpg" width="300"></a> | <br/>

● SinglePlayer 영상 -> CombatSystem 설명 <br/>
● SinglePlayer 영상 -> 멀티플레이 및 크래프팅 설명<br/>

# Packging File
● Link : https://drive.google.com/file/d/1Kx00sBZXGBgPO2W_-GXQI02bvHIhqV7Y/view?usp=drive_link <br/>

# 기술 설명
<br/>

## Boss
던전 마지막에 배치된 복잡한 AI<br/>

&nbsp;&nbsp;&nbsp;&nbsp;● BehaviorTree 동작방식 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● BossWidget 관리 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● BossBlackBoard 관리 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 네트워크 동기화 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Service Node, Decorator Node를 사용한 여러가지 조건 체크<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Blackboard과 BehaviorTree의 상호작용<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 각각 Task에서 독립적인 패턴 구현<br/>

> **Boss 설계도**<br/>

![Image](https://github.com/user-attachments/assets/b17883c1-38a0-46f5-8e1f-eea31f9bd5ad)<br/>

| Phase1 Nodes | Phase2 Nodes |
|:--:|:--:|
| ![Phase1](https://github.com/user-attachments/assets/b2c2124e-21c3-4aa2-9dfe-5cccb43684e6)<br/><sub></sub> | ![Phase2](https://github.com/user-attachments/assets/cfef0d85-6d41-4863-98f6-33b1b51c47c4)<br/><sub></sub> |

| Pattern Nodes | All Nodes |
|:--:|:--:|
| ![Phase3](https://github.com/user-attachments/assets/661f0aaf-756f-4a8a-81f1-82f447ea6222)<br/><sub></sub> | ![Phase4](https://github.com/user-attachments/assets/088dfdb6-a959-4a48-9530-d5d0f2578e5d)<br/><sub></sub> |<br/>
> **Service_Calculate Distance Node**
```cpp
#include "Boss/Service_CalculateDistanceBetween.h"
#include "Player/PixelCodeCharacter.h"
#include "Boss/BossApernia.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

UService_CalculateDistanceBetween::UService_CalculateDistanceBetween()
{
    NodeName = TEXT("Calculate Between Boss To Player");
    currentTime = 0.0f;
    timeToSelectPlayer = 10.0f; // 플레이어를 선택할 주기
}

void UService_CalculateDistanceBetween::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    currentTime += DeltaSeconds;

    // BossApernia 찾기
    ABossApernia* bossCharacter = Cast<ABossApernia>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossApernia::StaticClass()));
    if (!bossCharacter)
    {
        return;
    }
    if (currentTime >= timeToSelectPlayer)
    {
        currentTime = 0.0f;
        TArray<AActor*> foundCharacters;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

        if (foundCharacters.Num() > 0)
        {
            int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
            player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);
            if (player)
            {
                FVector playerLocation = player->GetActorLocation();
            }
        }
    }
    if (player)
    {
        float distance = FVector::Distance(bossCharacter->GetActorLocation(), player->GetActorLocation());
        OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), distance);
    }
}
```

## Enemy AI
맵에 배치된 기본적인 적AI<br/>

&nbsp;&nbsp;&nbsp;&nbsp;● BehaviorTree 동작방식 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● StateMachine 상반신 하반신 개별화 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 독자적 개인 시야각 적용 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 데미지 출력 HUD <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 여러가지 아이템 확률드랍 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 네트워크 동기화 <br/>
| Grux | Dog |
|:--:|:--:|
| <div align="center"> ![왼쪽 위](https://github.com/user-attachments/assets/df94cdcc-cfee-4583-9248-461bde2f5472) </div> | <div align="center"> ![오른쪽 위](https://github.com/user-attachments/assets/bbd4e4be-a4dd-4b1b-ad49-3ab117255f86) </div> |
| <div align="center"> ![왼쪽 아래](https://github.com/user-attachments/assets/4eaa73d2-e43c-45a0-a112-fc89318e229a) </div> | <div align="center"> ![오른쪽 아래](https://github.com/user-attachments/assets/8a78faf6-cc62-4232-8241-b12da5d8bef6) </div> |

> **Task_DogBartMoveToPlayer Node**
```cpp
#include "Task_DogBartMoveToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DogBartAIController.h"
#include "DogBart.h"
#include "NavigationSystem.h"


UTask_DogBartMoveToPlayer::UTask_DogBartMoveToPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Move To nearest Player");
}

EBTNodeResult::Type UTask_DogBartMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (BlackboardComp)
    {
        // 블랙보드에서 벡터 값을 가져옵니다.
        FVector NearestPlayerLocation = BlackboardComp->GetValueAsVector(TEXT("DetectedPlayerLoc"));

        if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
        {
            ADogBartAIController* dogBartController = Cast<ADogBartAIController>(OwnerComp.GetAIOwner());
            if (dogBartController)
            {
                
                dogBartController->MoveToLocation(NearestPlayerLocation);
            }
        }      
    }
    return EBTNodeResult::Failed;
}
```

> **Task_DogBartPatrol Node**
```cpp
#include "Task_DogBartPatrol.h"
#include "DogBartAIController.h"
#include "NavigationPath.h"
#include "AI/Navigation/NavigationTypes.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"

UTask_DogBartPatrol::UTask_DogBartPatrol(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("DogBart Patrol");
	bNotifyTick = true;
}

EBTNodeResult::Type UTask_DogBartPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TickTask(OwnerComp, NodeMemory, 0.0f);
	ADogBartAIController* dogBartController = Cast<ADogBartAIController>(OwnerComp.GetAIOwner());
	if (dogBartController)
	{
		FVector origin = dogBartController->GetPawn()->GetActorLocation();
		navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
		FVector randomLocation;
		if (navSys)
		{
			if (navSys->K2_GetRandomReachablePointInRadius(GetWorld(), origin, randomLocation, 1500.0))
			{
				dogBartController->MoveToLocation(randomLocation);
			}
		}
	}
	return EBTNodeResult::Succeeded;
}

void UTask_DogBartPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	currentTime += DeltaSeconds;
}
```

> **StateMachine**<br/>

![Image](https://github.com/user-attachments/assets/a255c6c9-c8b6-4ad0-b2cc-7c8a914b5bb9) </div>

## Crafting
크래프팅 시스템<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 데이터 테이블로 관리하는 쉬운 에셋추가 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 구조체를 사용한 쉬운 레시피, 수량 제어 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Name위젯 , Recipe위젯  CraftingWidget의 값 제어 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 소켓에 따른 스냅방식 HUD <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 프리뷰 메시를 사용한 시각적 표현 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Folliage Actor의 별도 인스턴스화 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Tag방식 <br/>

![Image](https://github.com/user-attachments/assets/fb6f9afd-9fa6-4dd4-b329-b30efa331693) </div>
![Image](https://github.com/user-attachments/assets/22e793f9-8c5b-4d7d-81cb-e34957802719) </div>




