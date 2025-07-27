// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interfaces/MainMenuWidget.h"
#include "Player/PixelCodeCharacter.h"
#include "Player/inventory/itemDragDropOperation.h"
#include "Player/inventory/ItemBase.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<APixelCodeCharacter>(GetOwningPlayerPawn()); // �⺻������ HUD�� �÷��̾� ��Ʈ�ѷ� �� �÷��̾�� ����Ǿ�����.
}

// ���콺�� Ŭ���ϰ� �巡���Ҷ� �����ͺ���
bool UMainMenuWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UitemDragDropOperation* ItemDragDrop = Cast <UitemDragDropOperation>(InOperation);

	if (PlayerCharacter && ItemDragDrop->SourceItem)
	{
		PlayerCharacter->PlayerInventory->FindMatchingItem(ItemDragDrop->SourceItem);
		PlayerCharacter->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
		return true;
	}
	return false;

	// Cast Operation to item drag drop, ensure player is valid, call drop item on player �÷��̾ �÷��̾�� ��ȿ�� ȣ�� ����׸����� Ȯ��

}