// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/inventory/InventoryComponent.h"
#include "Player/inventory/ItemBase.h"
#include "Player/PixelCodeCharacter.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
		// ...
	//UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent GetIsReplicated() : %s"), GetIsReplicated() ? TEXT("TRUE") : TEXT("FALSE"));
	if (GetIsReplicated() != true)
	{

		SetIsReplicated(true);
	}

	//UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent GetIsReplicated() : %s"), GetIsReplicated() ? TEXT("TRUE") : TEXT("FALSE"));
	if (GetIsReplicated() != true)
	{

		SetIsReplicated(true);
	}
}



void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if (ItemIn) // �����Ͱ� ��ȿ���� Ȯ��
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn)) // Ű�� �׸�ã��, �۵��ϸ� ��������� ��������.
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByPredicate([&ItemIn] // ����������
	(const UItemBase* InventoryItem)
		{

			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack(); // ID�� ���� Ǯ��������������
		}
	))
	{
		return *Result;
	}
	return nullptr;
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight()); // �������߷����� ���߷����� �߷��뷮�� 50�̰� 20������ϸ� 30�̵�, �ε��Ҽ����� ������ ���尡��������� ������.
	if (WeightMaxAddAmount >= RequestedAddAmount) // �ִ��߷��� �����߷����� ũ�ų����ٸ� ��û�� ���ڹ�ȯ
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity; // �����͸� �������� �ִ뽺��ũ�⸦ �����ͼ� ��������� ��

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack); // ���� �� ���� �� ��ȯ
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);// �����׸� ����
	OnInventoryUpdated.Broadcast();

}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity); // �׸��� �� ����

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove); // ��������, �׸��� ��������ǥ��, ������ �������� �� ���� ���ϴ� ���� �����ؼ� ��������

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

	OnInventoryUpdated.Broadcast();// ������Ʈ�ϰ� 

	return ActualAmountToRemove; // ������ ������ �� ��ȯ
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity)) // �κ��丮�� ��ġ�������� ����
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

void UInventoryComponent::RefreshInventory()
{
	OnInventoryUpdated.Broadcast();
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InputItem)
{
	// ��ȿ�� ����ġ�� �ִ��� Ȯ��, �뷮�̾������
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Item has invalid weight value."), InputItem->TextData.Name)); // �߰���������
	}

	// will the item weight overflow weight capacity �߰��ϸ� �뷮�ʰ�
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Item would overflow weight limit."), InputItem->TextData.Name));
	}

	// adding one more item would overflow slot capacity �׸��� �ϳ� �� �߰��ϸ� ���Կ뷮�ʰ�
	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. All inventory slots are full."), InputItem->TextData.Name));
	}

	// �� ��������� ����
	AddNewItem(InputItem, 1);

	return FItemAddResult::AddedAll(1, FText::Format(FText::FromString("Successfully added a single {0} to the inventory."), InputItem->TextData.Name));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight())) // ��û�� ���ڰ� �߸��ǰų� 0�� ������� Ȯ��
	{
		// ��ȿ���� ���� �׸� ������
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount; // �κ��丮�� �ֱ����� �����ڸ�

	// check if the input item already exists in the inventory and is not a full stack �׸��� �׸��� �̹� �κ��丮�� �ִ��� Ȯ���ϰ�
	UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn); // ������ id ���Ͽ� ������ ������ �׸����� Ȯ��

	// distribute item stack over existing stacks; ���ؽ���
	while (ExistingItemStack) // Loop�� ����ϴµ� null�� ����
	{
		// ���� ���Գ� ���� �Ѵ� ���
		// ��ü ������ ����� ���� ��ü ���� �ݾ��� �ƴ��� Ȯ���Ѵٴ� ���� ���� ��ü ������ ����� �� �ʿ��� ���� �׸��� ���� ����ϴ� ��.
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute); // �ִ밡 4�� 2�� ����ִ� ���� 2�� ���� �� ����, ���� ���
		// ���� ���� ��ü ���� ���� �� ������ �󸶳� ���� ���� ����� �� �ִ��� ���
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack); // ���԰��
 
		// ��ǰ�� ���� ���� ���� �뷮�� �ʰ����� �ʴ� ��
		if (WeightLimitAddAmount > 0)
		{
			// ���� ǰ�� ���� ���� �� ��� �� �߷� ����
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

			// �й��� ������ ����
			AmountToDistribute -= WeightLimitAddAmount;

			ItemIn->SetQuantity(AmountToDistribute);

			// TODO: Refine this logic since going over weight capacity should not ever be possible
			// ���� �ִ� ���Կ� ��������⶧���� ���� ��� �������ʿ� x
			if (InventoryTotalWeight >= InventoryWeightCapacity) 
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		else if (WeightLimitAddAmount <= 0)
		{
			if (AmountToDistribute != RequestedAddAmount)
			{
				// ���� ���ÿ� �׸��� �����ϸ� �� ��Ͽ� �����մϴ�.
				// �� �������� ü�� ������ ���� ���Դϴ�

				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}

			return 0;
		}
		if (AmountToDistribute <= 0) // ���������� ������ �ݾ��� 0���� �۰ų� �������, ���� ����. ������ �ڵ�� �Ϻι���
		{
			// all of the input item was distributed across existing stacks ���� ���ؿ� �л�Ǿ ���̻� ������ �׸��� �����Ƿ�
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}
		// �ش� �׸��� ��ȿ�� �κ� ������ ���� �����ִ��� Ȯ��
		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	// no more partial stacks found. check if a new stack can be added �� �̻� �κн��� �߰�x  �� ���� �ٷ� �߰��� �� �ִ��� Ȯ��
	if (InventoryContents.Num() + 1 <= InventorySlotsCapacity) // �������� ��ȣ +1�� ���� �뷮���� �۰ų� �������
	{
		// ���� �츮�� �̹� ���� �׸� �������� ��� �뷮�� ���� �� �ִ� ��ŭ �߰�
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn,AmountToDistribute);

		if (WeightLimitAddAmount > 0)
		{
			// ���� ���� �������� ������ ���� ���ѿ� ����.
			if (WeightLimitAddAmount < AmountToDistribute)
			{
				// �Է� �׸��� �����ϰ� �ùٸ� ����ŭ �� ������ �߰�
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountToDistribute);

				// �κ� ���ø� �߰��ǹǷ� ���纻�� ����ϴ�.
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}
			// ������ ������ ��ü �߰�
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
	}

	OnInventoryUpdated.Broadcast();
	return RequestedAddAmount - AmountToDistribute;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;
		//UItemBase *TempInfo = InputItem;
		// handle non-stackable items
		if (!InputItem->NumericData.bisStackable)
		{
			return HandleNonStackableItems(InputItem);
			//TempInfo->Quantity -= 1;
		}
		/*else
		{
			Char->DropedItem(TempInfo);
		}*/

		// handle stakckable �� �߰�
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(FText::FromString("Successfully added {0} {1} to the inventory."), InitialRequestedAddAmount, InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(FText::FromString("Partial amount of {0} added to the inventory. Number added = {1}"), InputItem->TextData.Name, StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0) // �߰��� ���� 0���� ������ �ȵ�����
		{
			return FItemAddResult::AddedNone(FText::Format(FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."), InputItem->TextData.Name));
		}
	}

	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed."));
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;

	

	if (Item->bIsCopy || Item->bIsPickup)
	{
		// �׸��� �̹� �纻�̰ų� �Ⱦ��� ���
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// �׷����������
		NewItem = Item->CreateItemCopy(); // �׸� ���纻�� �����ϰ� ȣ��
	}

	NewItem->OwningInventory = this; // �κ��丮�� �������� �������
	NewItem->SetQuantity(AmountToAdd);	// �߰��� ��������

	InventoryContents.Add(NewItem);	// �� �׸� �߰�
	InventoryTotalWeight += NewItem->GetItemStackWeight();// ����ġ ����
	OnInventoryUpdated.Broadcast(); // ��� UI��ҿ� ������� ����

}




