// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/inventory/ItemBase.h"
#include <../../../../../../../Source/Runtime/Core/Public/Math/UnrealMathUtility.h>
#include "Player/PixelCodeCharacter.h"
#include "CreateItemData.h"
#include "Player/inventory/InventoryComponent.h"


// ���� -------=========================================================------------------------------------

TArray<FCraftItem> UItemBase::GetAllCrafting()
{
	return Crafting;
}


FCraftItemInfo UItemBase::GetCraftItemInfoBasedOn(EItemName Name)
{
	return CraftItemData[Name];
}



UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false) // �����ڰ� �����Ǵ� ���ÿ� ���� �־���.
{


}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass()); // ���ο� ������ �������, ����Ŭ������ ����.

	ItemCopy->ID = this->ID; // �����ϴ� ������ �ǹ�.
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->bIsCopy = true;
	ItemCopy->ItemName = this->ItemName;
	ItemCopy->Buildtypes = this->Buildtypes;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity) 
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bisStackable ? NumericData.MaxStackSize : 1); // ��, ����

		if (OwningInventory)
		{
			if (Quantity <= 0)
			{
				OwningInventory->RemoveSingleInstanceOfItem(this); //���� 0 ������ �ǹ̾�� �� �׸��� ������.
			}
		}
	}

}

void UItemBase::Use(APixelCodeCharacter* Character)
{


}

void UItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*DOREPLIFETIME(UItemBase, Quantity);
	DOREPLIFETIME(UItemBase, ID);
	DOREPLIFETIME(UItemBase, ItemType);
	DOREPLIFETIME(UItemBase, ItemQuality);
	DOREPLIFETIME(UItemBase, ItemName);
	DOREPLIFETIME(UItemBase, ItemStatistics);
	DOREPLIFETIME(UItemBase, TextData);
	DOREPLIFETIME(UItemBase, NumericData);
	DOREPLIFETIME(UItemBase, AssetData);
	DOREPLIFETIME(UItemBase, CraftItemData);
	DOREPLIFETIME(UItemBase, Crafting);*/
}