// Fill out your copyright notice in the Description page of Project Settings.


#include "PCodeSaveGame.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "DataTypes.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "Building.h"


// Fill out your copyright notice in the Description page of Project Settings.
//#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerState.h>

//void APC_Gameplay::BeginPlay()
//{
//    Super::BeginPlay();
//
//    // �÷��̾� ������Ʈ�� ��ȿ���� �˻��ϰ� ��ȿ�� ��쿡�� Setup_PC �Լ��� ȣ��
//    if (ValidatePlayerState())
//    {
//        Setup_PC();
//    }
//}
//void APC_Gameplay::ServerRPC_Spawn_Character_Implementation()
//{
//    UE_LOG(LogTemp, Warning, TEXT("2 APC_Gameplay::ServerRPC_Spawn_Character"));
//    Cast<ASAModeBase>(GetWorld()->GetAuthGameMode())->Spawn_Character(this);
//}
//
//bool APC_Gameplay::ValidatePlayerState()
//{
//    // Ensures that the player state is Valid before we setup the PlayerController
//    // �÷��̾� ��Ʈ�ѷ��� �����ϱ� ����, �÷��̾� ������Ʈ�� ��ȿ���� �����Ѵ�.
//    FTimerHandle TimerHandle_PlayerStateCheck;
//
//    if (PlayerState->IsValidLowLevel())
//    {
//        UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState->IsValid"));
//        return true;
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState->Is Not Valid"));
//        GetWorld()->GetTimerManager().SetTimer(TimerHandle_PlayerStateCheck, [&]() {
//            if (ValidatePlayerState())
//            {
//                UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState �ٽ� validate!!!!!!!!"));
//                Setup_PC();
//            }
//            }, 0.2f, false);
//
//        return false;
//    }
//}
//
//void APC_Gameplay::Setup_PC()
//{
//    if (IsLocalPlayerController())
//    {
//        UE_LOG(LogTemp, Warning, TEXT("1 APC_Gameplay::Setup_PC()"));
//        ServerRPC_Spawn_Character();
//    }
//}
//���� ��忡�� Client RPC���� ������ ���̺� ���ӿ� ����� ĳ���� Ŀ������ �ҷ���
//void ASAModeBase::Spawn_Character(APlayerController* player)
//{
//    // spawn character �� �� pawn destroy�ϰ� �ٽ� possess �ϴ� ������ �ִµ� �ϴ� �����ϰ���
//    UE_LOG(LogTemp, Warning, TEXT("3 ASAModeBase::Spawn_Character"));
//
//    // player�� null���� Ȯ��
//    if (!player)
//    {
//        UE_LOG(LogTemp, Error, TEXT("Player controller is null"));
//        return;
//    }
//
//    // ATestPlayer�� ĳ��Ʈ�� �����ϴ��� Ȯ��
//    ATestPlayer* TestPlayer = Cast<ATestPlayer>(player->GetPawn());
//    if (!TestPlayer)
//    {
//        UE_LOG(LogTemp, Error, TEXT("Cast to ATestPlayer failed"));
//        return;
//    }
//
//    TestPlayer->ClientRPC_Init_Appearance();
//}
//�÷��̾� cpp->�ҷ��� ĳ���� Ŀ������ ���������� ���� ��
//void ATestPlayer::ClientRPC_Init_Appearance_Implementation()
//{
//    UE_LOG(LogTemp, Warning, TEXT("4 ATestPlayer::ClientRPC_Init_Appearance_Implementation()"));
//    ATestPlayer::Get_Player_Appearance();
//}
//void ATestPlayer::Get_Player_Appearance()
//{
//    UE_LOG(LogTemp, Warning, TEXT("5 ATestPlayer::Get_Player_Appearance()"));
//    Get_Character();
//    Get_Skins();
//    Get_Eyes();
//    Get_Accessory();
//    Get_Top();
//    Get_Bottom();
//    Get_Outer();
//    Get_Dress();
//}
//void ATestPlayer::Get_Character()
//{
//    UE_LOG(LogTemp, Warning, TEXT("6 ATestPlayer::Get_Character()"));
//    FStructure_Player_Appearance_Result result = UFL_General::Get_Player_Appearance();
//    if (result.bSuccess)
//    {
//        ServerRPC_Update_Character(result.Player_Appearance.Character.Mesh);
//    }
//    else
//    {
//        return;
//    }
//}
//void ATestPlayer::ServerRPC_Update_Character_Implementation(USkeletalMesh* _mesh)
//{
//    UE_LOG(LogTemp, Warning, TEXT("7 ATestPlayer::ServerRPC_Update_Character"));
//    Current_SkeletalMesh = _mesh;
//    OnRep_Current_SkeletalMesh();
//}
//void ATestPlayer::OnRep_Current_SkeletalMesh()
//{
//    if (Current_SkeletalMesh->IsValidLowLevelFast())
//    {
//        UE_LOG(LogTemp, Warning, TEXT("8 ATestPlayer::OnRep_Current_SkeletalMesh()"));
//        GetMesh()->SetSkinnedAssetAndUpdate(Current_SkeletalMesh);
//
//        GetMesh()->SetVisibility(true);
//        SM_Outer->SetVisibility(false);
//    }
//}

// void UPCodeSaveGame::AddBuildingData(FBuildingData param)
// {
// 	//.Add(param);
// 	//UGameplayStatics::SaveGameToSlot(this, slotName, 0);
// }
// 
// void UPCodeSaveGame::SaveGame()
// {
//     UPCodeSaveGame* SaveGameInstance = Cast<UPCodeSaveGame>(UGameplayStatics::CreateSaveGameObject(UPCodeSaveGame::StaticClass()));
//     if (SaveGameInstance)
//     {
//         // ���忡�� ��� ���͸� ������
//         TArray<ABuilding*> ActorsToSave;
//         //UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuilding::StaticClass(), ActorsToSave);
// 
//         for (TActorIterator<ABuilding> var(GetWorld()); var; ++var)
//         {
//             ActorsToSave.Add( *var);
//         }
// 
//         // ���͵��� ���� ����
//         for (ABuilding* Actor : ActorsToSave)
//         {
//             if (Actor)
//             {
//                 FBuildingData ActorData;
//                 ActorData.ABuilding = Actor->GetClass();
//                 ActorData.BuildingLocation = Actor->GetActorLocation();
//                 ActorData.BuildingRotation = Actor->GetActorRotation();
// 
//                 // ���� �����͸� ���̺� ���� �ν��Ͻ��� �߰�
//                 SaveGameInstance->SavedActors.Add(ActorData);
//             }
//         }
// 
//         // ���̺� ���� ���Կ� ����
//         UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("BuildingDataStorage"), 0);
//     }
//     else
//     {
//         UE_LOG(LogTemp, Warning, TEXT("Failed to create save game instance"));
//     }
// 
// }



