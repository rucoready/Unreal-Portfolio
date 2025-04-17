// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/pixelPlayerState.h"
#include "Player/StateComponent.h"
#include "MyGameModeBase.h"
#include "Player/Widget/NormallyWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Player/PlayerStatWidget.h"
#include "PCodePlayerController.h"
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>




ApixelPlayerState::ApixelPlayerState()
{
    bReplicates = true;
}

void ApixelPlayerState::SetaddUpEXP(float AcquireEXP)
{
    maxEXP();

    // ���� ����ġ �߰�
	currentEXP += AcquireEXP;
    auto* GM = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GM)
    {
    	// GameMode�� �����Ͽ� ���ϴ� �۾� ����
    	GM->EXPmanagement(AcquireEXP, this);
    }
    // ����ġ ������Ʈ �� �ִ� ����ġ üũ
    
    // OnRep �Լ� ȣ��
    OnRep_currentEXP(currentEXP);
    
}

void ApixelPlayerState::LevelUP()
{
    Level += 1;
    maxEXP(); // ���� �� �� �ִ� ����ġ ������Ʈ

   auto* APc = Cast<APCodePlayerController>(GetPawn()->GetController());
   if (APc != nullptr)
   {
       APc->FullExp();
   }
}

void ApixelPlayerState::InitPlayerData()
{
    Level = 1;
    maxEXP(); // �ʱ�ȭ �� �ִ� ����ġ ����
    currentEXP = 0.0f;
}

float ApixelPlayerState::GetCurrentExp() const
{
    

    return currentEXP;
}

int32 ApixelPlayerState::GetCharacterLevel() const
{
    return Level;
}

void ApixelPlayerState::maxEXP()
{
    if (Level == 1)
    {
        totalEXP = 150;
    }
    else if (Level == 2)
    {
        totalEXP = 400;
    }
    else if (Level == 3)
    {
        totalEXP = 650;
    }
    else if (Level == 4)
    {
        totalEXP = 900;
    }
    else if (Level == 5)
    {
        totalEXP = 1150;
    }
}

void ApixelPlayerState::OnRep_totalEXP(int32 OldEXP)
{
    // totalEXP�� ����� �� ������ ���� �ۼ�
    //UE_LOG(LogTemp, Warning, TEXT("Total EXP changed from %d to %d"), OldEXP, totalEXP);
}

void ApixelPlayerState::OnRep_currentEXP(float OldEXP)
{
    // currentEXP�� ����� �� ������ ���� �ۼ�
	//UE_LOG(LogTemp, Warning, TEXT("Current EXP changed from %f to %f"), OldEXP, currentEXP);
}

