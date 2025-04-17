// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interfaces/InventoryPanel.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WrapBox.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "Player/PixelCodeCharacter.h"
#include "Player/inventory/InventoryComponent.h"
#include "Player/Inventory/InventoryItemSlot.h"
#include "Player/inventory/itemDragDropOperation.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<APlayerOrganism>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory(); // �κ��丮�� �������� �κ��丮 �������⸦ ���
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory); // �κ��丮 �г����� -> �κ��丮 ���ΰ�ħ
			SetInfoText();
		}
	}

}

// ���� ������ �뷮�� �����ϱ� ���Ѱ�
void UInventoryPanel::SetInfoText() const
{
	// ����
	const FString WeightInfoValue
	{
		FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + "/"
		+ FString::SanitizeFloat(InventoryReference->GetWeightCapacity())
	};


	// �뷮
	const FString CapacityInfoValue
	{
		FString::FromInt(InventoryReference->GetInventoryContents().Num()) + "/"
		+ FString::FromInt(InventoryReference->GetSlotsCapacity())
	};

	WeightInfo->SetText(FText::FromString(WeightInfoValue));
	CapacityInfo->SetText(FText::FromString(CapacityInfoValue));

}

void UInventoryPanel::RefreshInventory()
{
	if (InventoryReference && InventorySlotClass)
	{
		InventoryPanel->ClearChildren();

		for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}

		SetInfoText();
	}

}


bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// �κ��丮 �ȿ� �巡�׾� ���
	const UitemDragDropOperation* ItemDragDrop = Cast<UitemDragDropOperation>(InOperation);

	if (ItemDragDrop->SourceItem && InventoryReference)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Detected an item drop on InventoryPanel."))

		// returning true will stop the drop operation at this widget
		return true;
	}
	// returning false will cause the drop operation to fall throungt to underlying widgets (if any)
	return false;
}

void UInventoryPanel::SetTargetCharacter(APlayerOrganism* target)
{
	PlayerCharacter = target;

	InventoryReference = PlayerCharacter->GetInventory(); // �κ��丮�� �������� �κ��丮 �������⸦ ���
	if (InventoryReference)
	{
		RefreshInventory();
	}
}