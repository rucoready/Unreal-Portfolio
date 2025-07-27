// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/Interfaces/InteractionInterface.h"
#include "CreateItemData.h"
#include "DataTypes.h"
#include "Pickup.generated.h"


class UDataTable;
class UItemBase;
class APlayerOrganism;
class ABuilding;
class UMaterialInstance;

UCLASS()
class PIXELCODE_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================
	

	//================================================================================
	// FUNCTIONS
	//================================================================================
	APickup();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity); // �Ⱦ� �ʱ�ȭ


	UFUNCTION(Client, Reliable)
	void ClientRPC_InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity); // ��� �ʱ�ȭ

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity); // ��� �ʱ�ȭ

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	

protected:
	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;


	UPROPERTY(EditDefaultsOnly, Category = "Pickup | Item Reference")
	UItemBase* ItemReference; // �Ⱦ��� ����Ű�� �׸�

	UPROPERTY(EditDefaultsOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity; // ����

	UPROPERTY(EditDefaultsOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData; // �ν��Ͻ� ��ȣ�ۿ밡��

	// �׽�Ʈ 
	/*UPROPERTY(EditAnywhere, Category = "Item")
	TMap<uint8, TSubclassOf<AActor>> ItemTemplates;*/

	UPROPERTY(EditAnywhere, Category = "Item")
	EItemName ItemName;

	
	UPROPERTY(EditDefaultsOnly, Category = KSH)
	TArray<FBuildingVisualType> BuildingTypes;

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	TSubclassOf<ABuilding> BuildingClass;

	
	UPROPERTY(EditDefaultsOnly, Category = KSH)
	UMaterialInstance* MaterialFalse;

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	UMaterialInstance* MaterialTrue;


	//================================================================================
	// FUNCTIONS
	//================================================================================
	

#if WITH_EDITOR // ��ũ��
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:

	UPROPERTY(EditDefaultsOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	void SetInput(const APlayerOrganism* Taker);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(APixelCodeCharacter* PlayerCharacter) override;
	void UpdateInteractableData();

	void TakePickup(const APlayerOrganism* Taker); // ĳ������ �κ��丮�� �������̽��ؾ���


	FInteractableData GetItemInfo();




};
