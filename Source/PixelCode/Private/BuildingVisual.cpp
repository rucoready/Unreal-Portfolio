// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingVisual.h"
#include "Building.h"
#include "Components/StaticMeshComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h>
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include "PCodeSaveGame.h"


// Sets default values
ABuildingVisual::ABuildingVisual()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	BuildMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildMeshComponent"));
	RootComponent = BuildMesh;

	BuildingTypeIndex = 0;

	bMaterialIsTrue = false;
	bReturnedMesh = true;
}

// Called when the game starts or when spawned
void ABuildingVisual::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorHiddenInGame(true);

	if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
	{
		BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh); // �ε��� �ʱ� ������ 0 = Foundation
	}

	if (MaterialTrue)
	{
		bMaterialIsTrue = true;
		BuildMesh->SetMaterial(0, MaterialTrue);
	}
}

ABuilding* ABuildingVisual::GetHitBuildingActor(const FHitResult& HitResult)
{
	return Cast<ABuilding>(HitResult.GetActor());
}

void ABuildingVisual::SetMeshTo(EBuildType BuildType)
{
 	//UE_LOG(LogTemp, Warning, TEXT("SetMeshTo :"));

	bReturnedMesh = false;
	for (const FBuildingVisualType& Building : BuildingTypes)
	{
		if (Building.BuildType == BuildType)
		{
			BuildMesh->SetStaticMesh(Building.BuildingMesh);
			return;
		}
	}
}

void ABuildingVisual::ReturnMeshToSelected()
{
	//UE_LOG(LogTemp, Warning, TEXT("ReturnMeshToSelected"));

	bReturnedMesh = true;
	if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
	{
		BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
	}
}

void ABuildingVisual::SetBuildPosition(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		auto Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (Pc)
		{
			pc = Cast<APixelCodeCharacter>(Pc->GetPawn());
			FRotator ControlRotation = Pc->GetControlRotation();

			// Z�� ȸ���� ��Ʈ�ѷ��� ȸ������ ����
			FRotator NewRotation = GetActorRotation();
			NewRotation.Yaw = ControlRotation.Yaw;
			SetActorRotation(NewRotation);		//�÷��̾� ������ ���� �޽ð� �̵��ϵ���
		}
		SetActorHiddenInGame(false); // BuildingVisual ������ ���̱�
		InteractingBuilding = GetHitBuildingActor(HitResult); // HitResult�� Building�� Actor�� ĳ����
 
 		// #19 ���� ���� ������Ű��
 		if (InteractingBuilding)
 		{
 			if (!bReturnedMesh)
 			{
 				ReturnMeshToSelected(); // ���⼭ bReturnMesh = true
 			}
 			
 			SocketData = InteractingBuilding->GetHitSocketTransform(HitResult, BuildingTypes[BuildingTypeIndex].FilterCharacter, 100.0f); // ���� ���� ����
 
 			if (!SocketData.SocketTransform.Equals(FTransform()))
 			{
 				SetActorTransform(SocketData.SocketTransform); // �ش� ���Ͽ� �´� �������� ȸ��!!
 
 				if (MaterialTrue && !bMaterialIsTrue)
 				{
 					bMaterialIsTrue = true;
 					BuildMesh->SetMaterial(0, MaterialTrue);		// ������ �ʷϻ�����
 				}
 				return;
 			}
 			else
 			{
 				if (MaterialFalse && bMaterialIsTrue)
 				{
 					bMaterialIsTrue = false;
 					BuildMesh->SetMaterial(0, MaterialFalse);		// ������ ����������
 				}
 				SetActorLocation/*AndRotation*/(HitResult.Location/*, GetActorRotation()*/);		// ������ġ ȸ������ �ʱ�ȭ
  			}
 		}
 		else
 		{
 			if (bReturnedMesh)
 			{
//  	&%&%&%			if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
//  				{
//  					BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
//  					//BuildMesh->SetStaticMesh(ItemReference->Buildtypes[BuildingTypeIndex].BuildingMesh);
//  	&%&%&%			}
 			}

			Loc = HitResult.ImpactPoint;
			NewLoc = Loc;
			NewLoc.Z =Loc.Z + 40.f;
//  			SetActorLocation(HitResult.ImpactPoint);
			SetActorLocation(NewLoc);		// ������ 40cm ���� ������ ����
 		}
	}
	else
	{
		InteractingBuilding = nullptr;
		SetActorHiddenInGame(true); // ����Ʈ���̽� �Ÿ� �ȿ� HitResult�� ������ ������ ����
	}
}

void ABuildingVisual::SpawnBuilding()
{
	bItemQuantityValid = false;
 	auto Pc = Cast<APlayerOrganism>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
 	if (Pc)
 	{
 		TArray<UItemBase*> InventoryContentSArray = Pc->PlayerInventory->GetInventoryContents();
 		//uint8 BulidAmount = Recipe.Amount;
 		uint8 Index = 0;
 		TArray<uint8> RemoveedIndex;
 
 		if (InventoryContentSArray.IsValidIndex(Index))
 		{
 			for (UItemBase* Item : InventoryContentSArray)
 			{
 				if (Item && Item->Buildtypes == BuildingTypes[BuildingTypeIndex].BuildType)
 				{
 					if (Item->Quantity > 0)
 					{
						bItemQuantityValid  = true;		// ������ ���� ��ŭ�� ���� ���ɸ��
					}
				}
			}
		}
	}
	// ABuilding �� ������ �ƴ� �� = �������簡 preview ������ ��
	if (BuildingClass && !IsHidden() && bItemQuantityValid)	// ������ ���� ��ŭ�� ���� ���ɸ�� -> ġƮ���� �ٲٷ��� !bItemQuantityValid �� ����
	{
		// ABuilding �ν��Ͻ� = �������簡 ���� ��
		if (InteractingBuilding)
		{
			// preview�� �ʷ��� ��
			if (bMaterialIsTrue)
			{
				// ABuilding Ŭ������ AddInstance() ȣ��
				InteractingBuilding->AddInstance(SocketData, BuildingTypes[BuildingTypeIndex].BuildType);
				//UE_LOG(LogTemp, Warning, TEXT("---------------------BUILDINGVISUAL Add Instance"));
			}
		}
		else
		{
			//GetWorld()->SpawnActor<ABuilding>(BuildingClass, Loc, GetActorRotation());
			ABuilding* AbuildingClass = GetWorld()->SpawnActor<ABuilding>(BuildingClass, Loc, GetActorRotation());
			//UE_LOG(LogTemp, Warning, TEXT("---------------------BUILDINGVISUAL Spawn Actor"));
 
								

			UPCodeSaveGame* castSave = Cast<UPCodeSaveGame>(UGameplayStatics::CreateSaveGameObject(UPCodeSaveGame::StaticClass()));
			UPCodeSaveGame* castLoad = Cast<UPCodeSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("BuildingDataStorage"), 0));
								
			if (castLoad)
			{
				FBuildingActorData BuildingActorData;
				BuildingActorData.ABuilding = AbuildingClass->GetClass();
				BuildingActorData.BuildingLocation = AbuildingClass->GetActorLocation();
				BuildingActorData.BuildingRotation = AbuildingClass->GetActorRotation();

				castLoad->SavedActors.Add(BuildingActorData);
				UGameplayStatics::SaveGameToSlot(castLoad, TEXT("BuildingDataStorage"), 0);

				int32 arrnum = castLoad->SavedActors.Num();
				//UE_LOG(LogTemp, Warning, TEXT("-------------SAVEGAME__ACTOR Load :: %d "), arrnum);
			}

 			else if (castSave)
 			{
 				FBuildingActorData BuildingActorData;
 				BuildingActorData.ABuilding = AbuildingClass->GetClass();
 				BuildingActorData.BuildingLocation = AbuildingClass->GetActorLocation();
 				BuildingActorData.BuildingRotation = AbuildingClass->GetActorRotation();
 
 				castSave->SavedActors.Add(BuildingActorData);
 				UGameplayStatics::SaveGameToSlot(castSave, TEXT("BuildingDataStorage"), 0);

				int32 arrnum = castSave->SavedActors.Num();
				//UE_LOG(LogTemp, Warning, TEXT("-------------SAVEGAME__ACTOR Cast :: %d "), arrnum);
 			}
		}
	}
}

void ABuildingVisual::DestroyInstance(const FHitResult& HitResult)
{
	if (InteractingBuilding)
	{
		if (HitResult.bBlockingHit)
		{
			if (UInstancedStaticMeshComponent* InstancedStaticMeshComponent = Cast<UInstancedStaticMeshComponent>(HitResult.GetComponent()))
			{
				FBuildingSocketData BuildingSocketData;
				BuildingSocketData.InstancedComponent = InstancedStaticMeshComponent;
				BuildingSocketData.Index = HitResult.Item;

				InteractingBuilding->DestroyInstance(BuildingSocketData);

			}
		}
	}
}

void ABuildingVisual::CycleMesh()
{
	//if (bReturnedMesh)
  	
		if (++BuildingTypeIndex >= BuildingTypes.Num())
 		{
 			BuildingTypeIndex = 0;
 		}
		// ��������[#]�� �޽ø� preview �޽÷� �����ϱ�
		if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
		{
			BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
		}


// 	auto Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
// 	if (Pc  /* ROLE_AutonomousProxy*/)
// 	{
// 		pc = Cast<APixelCodeCharacter>(Pc->GetPawn());
// 		float wheelAxisVal = pc->wheelAxis;
// 
// 		// �������� �ε��� ��ũ�ѷ� ������
// 		if(wheelAxisVal > 0)
// 		{
// 			if (BuildingTypeIndex < BuildingTypes.Num() - 1)
// 			{
// 				++BuildingTypeIndex;
// 			}
// 			else
// 			{
// 				BuildingTypeIndex = 0; // �迭�� ó������ ��ȯ
// 			}
// 		}
// 		else if (wheelAxisVal < 0) // ��ũ�� ���� �ø�
// 		{
// 			if (BuildingTypeIndex > 0)
// 			{
// 				--BuildingTypeIndex;
// 			}
// 			else
// 			{
// 				BuildingTypeIndex = BuildingTypes.Num() - 1; // �迭�� ������ ��ȯ
// 			}
// 		}
// 
// 		// ��������[#]�� �޽ø� preview �޽÷� �����ϱ�
// 		if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
// 		{
// 			BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
// 		}
//  	}
}

void ABuildingVisual::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABuildingVisual, BuildingTypeIndex);
	DOREPLIFETIME(ABuildingVisual, BuildMesh);
	DOREPLIFETIME(ABuildingVisual, BuildingClass);
	DOREPLIFETIME(ABuildingVisual, BuildingTypes);
	DOREPLIFETIME(ABuildingVisual, MaterialFalse);
	DOREPLIFETIME(ABuildingVisual, MaterialTrue);
	DOREPLIFETIME(ABuildingVisual, bMaterialIsTrue);
	DOREPLIFETIME(ABuildingVisual, InteractingBuilding);
	DOREPLIFETIME(ABuildingVisual, SocketData);
	DOREPLIFETIME(ABuildingVisual, bReturnedMesh);
}


