// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreateItemData.h"
#include "ItemStorage.generated.h"

UCLASS()
class PIXELCODE_API AItemStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemStorage();

	// id�� ������� Ŭ���� ��������
	TSubclassOf<AActor> GetTemplateOfItem(uint8 Id);


	TSubclassOf<AActor> GetTemplateOfItem(EItemName ItemName);
	

	// �׽�Ʈ =============================================================
	UPROPERTY(EditInstanceOnly, Category = "Item")
	UDataTable* ItemDataTable;

	// ��� ����
	TArray<FCraftItem> GetAllCrafting();

	// EItemName ��� ũ����Ʈ ǰ�� ����
	FCraftItemInfo GetCraftItemInfoBasedOn(EItemName Name);

	//  ���ø�
	UPROPERTY(EditAnywhere, Category = "Item")
	TMap<uint8, TSubclassOf<AActor>> ItemTemplates;
	

	//  �����͵� , EItemName ��� �ؽ�Ʈ �� ������
	UPROPERTY(EditAnywhere, Category = "Item")
	TMap<EItemName, FCraftItemInfo> CraftItemData;

	UPROPERTY(EditAnywhere, Category = "Item")
	TArray<FCraftItem> Crafting;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	


	
};
