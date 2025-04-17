// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UMainMenuWidget; // ���� ���ƴ��� �˾ƺ�������, ������� �߰� ���ص� ��.
class UInteractionWidget;
class UPlayerStatWidget;
//class ULootPanel;
class UCraftingWidget;
class UBulidWIdget;
struct FInteractableData;

// HUD�� ������ �����ϱ� �Ϻ��� ���
/**
 *
 */

/**
 * 
 */
UCLASS()
class PIXELCODE_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	

public:
	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================
	 
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;



	bool bIsMenuVisible; // ���ظ�忡 �ִ� �����޴� ��ȯX�ϱ����� �Ұ�

	//================================================================================
	// FUNCTIONS
	//================================================================================
	APlayerHUD();


	void DisplayMenu(); // ǥ�ø޴����
	void HideMenu(); // �޴������
	void ToggleMenu();


	void ShowInteractionWidget()const;
	void HideInteractionWidget()const;
	void UpdateInteractionWidget(const FInteractableData& InteractableData)const; // ��ȣ�ۿ밡�ɵ�����

	

	//���� ------------------------------====================================================================
	void ShowOrHideCrafting();
	void HideCrafting();
	bool bIsCreatVisible;
	void ToggleCreate();
	void creatwidgets();

	 
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UCraftingWidget> CraftingClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	UCraftingWidget* Crafting;


	void ShowBuilding();
	void HideBuildinging();
	bool bIsbuildVisible;
	void ToggleBuilding();
	void Buildwidgets();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	UBulidWIdget* Building;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UBulidWIdget> BuildingClass;
	
	

protected:

	//================================================================================
	// PROPERTIES & VARIABLES
	UPROPERTY()
	UMainMenuWidget* MainManuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;


	//================================================================================

	//================================================================================
	// FUNCTIONS
	//================================================================================
	virtual void BeginPlay() override;

};
