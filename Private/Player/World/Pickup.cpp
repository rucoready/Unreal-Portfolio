// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/World/Pickup.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include "Player/inventory/ItemBase.h"
#include "Player/PixelCodeCharacter.h"
#include "Player/Inventory/InventoryComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;


	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);

	ItemDataTable = ConstructorHelpers::FObjectFinder<UDataTable>(TEXT("/Script/Engine.DataTable'/Game/YoHan/DataTable/itembase_-_itembase.itembase_-_itembase'")).Object;

}


void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UItemBase::StaticClass(), ItemQuantity); // ��������
}


void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// �Ⱦ� �������� �ʱ�ȭ
void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone()) // ���� ���������� Ȯ�� ,�����������
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		// ������ ������ ������ ���� ����
		if (ItemReference == nullptr)
		{
			ItemReference = NewObject<UItemBase>(this, BaseClass);
		}

		// ������ �����ͷ� ItemReference�� �ʱ�ȭ
		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->AssetData = ItemData->AssetData;
		ItemReference->ItemName = ItemData->ItemName;
		ItemReference->Buildtypes = ItemData->Buildtypes;

		// ���� ����
		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		// �޽ÿ� ������ �������� �޽��� ����
		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		// ��ȣ�ۿ� ������ ������Ʈ
		UpdateInteractableData();
	}
}

void APickup::ClientRPC_InitializeDrop_Implementation(UItemBase* ItemToDrop, const int32 InQuantity)
{
	
	//NetMulticastRPC_InitializeDrop();
}

void APickup::NetMulticastRPC_InitializeDrop_Implementation(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight(); // �׸��Ӽ���
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh); // ����� �׸����� �����޽�Ȯ��

	UpdateInteractableData();
}

void APickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InstanceInteractableData.ItemName = ItemReference->ItemName;
	InteractableData = InstanceInteractableData;


}

void APickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}

}

void APickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void APickup::Interact(APixelCodeCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter); // �Ⱦ���û, �÷��̾�ĳ���� ����
	}
}

void APickup::TakePickup(const APlayerOrganism* Taker)
{
	if (!IsPendingKillPending()) // ���� ��� ������ Ȯ��
	{
		if (ItemReference)
		{
			if (UInventoryComponent* PlayerInventory = Taker->GetInventory())
			{
				// �÷��̾� �κ��丮�� �׸��� �߰��� ����
				 
				// �κ��丮�� ������ �߰� �� ��� ó��
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				switch (AddResult.operationResult)
				{
				case EItemAddResult::IAR_NoItemAdded:
					break; // �ƹ��͵���������
				case EItemAddResult::IAR_PartialAmountItemAdded:
				{
					UpdateInteractableData(); // �Ⱦ�����������Ʈ

					auto charCheck = Cast<APixelCodeCharacter>(Taker);
					if (charCheck != nullptr)
					{
						// ����������Ʈ
						charCheck->UpdateInteractionWidget();
					}
					break;
				}
				case EItemAddResult::IAR_AllItemAdded:
					Destroy();  //�������� ��� �߰��Ǹ� �Ⱦ� ��ü ����
					break;
				default:
					break;
				}
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("Player inventory conponent is null!!")); // �κ��丮�� ��
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Pickup internal item reference was somenow null!!")); // �Ⱦ��� ��
		}
	}

}

//void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	//Super::PostEditChangeProperty(PropertyChangedEvent);
//
//	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
//	// �̸��� ��ȿ�ϸ� �̸���ȯ, ��ȿ���������� �̸������� ��ȯ
//
//	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickup, DesiredItemID)) // �繰�� ��ȿ���� Ȯ���ϴ� ��ũ��
//	{
//		if (ItemDataTable)
//		{
//			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString())) // ��ȿ�� ���� ������ ��� ����ȭ
//			{
//				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh); // �Ⱦ��޽ø� �����͸޽÷� ����
//			}
//		}
//	}
//
//}

void APickup::SetInput(const APlayerOrganism* Taker)
{
	if (ItemDataTable)
	{
		InitializePickup(UItemBase::StaticClass(), ItemQuantity);
		TakePickup(Taker);

		//if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString())) // ��ȿ�� ���� ������ ��� ����ȭ
		//{
		//	PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh); // �Ⱦ��޽ø� �����͸޽÷� ����

		//	ItemQuantity = (ItemQuantity == 0) ? 1 : ItemQuantity;


		//}
	}
}

FInteractableData APickup::GetItemInfo()
{
	return FInteractableData();
}





