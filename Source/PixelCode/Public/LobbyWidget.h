// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()


	UFUNCTION()
	void OnMyClickCreateRoom();

	UFUNCTION()
	void OnValueChanged(float value);

	virtual void NativeConstruct() override;


	

	UPROPERTY()
	class UPCodeGameInstance* gi;


	// �ڷ� ��ư �� �Լ�
	UPROPERTY(EditDefaultsOnly, Meta=(BindWidget))
	class UButton* Button_Back1;

	
	UPROPERTY(EditDefaultsOnly, Meta=(BindWidget))
	class UButton* Button_Back2;

	UFUNCTION()
	void OnMyClickBackRoom();


	// ũ������Ʈ �� UI
	UPROPERTY(EditDefaultsOnly, Meta=(BindWidget))
	class UEditableText* Edit_RoomName;

	UPROPERTY(EditDefaultsOnly, Meta=(BindWidget))
	class USlider*  Slider_PlayerCount;

	UPROPERTY(EditDefaultsOnly, Meta=(BindWidget))
	class UButton* Button_CreatRoom;

	UPROPERTY(EditDefaultsOnly, Meta=(BindWidget))
	class UTextBlock* Text_PlayerCount;

	// ���� ��

	UPROPERTY(EditDefaultsOnly, Meta=(BindWidget))
	class UWidgetSwitcher* SwitcherUI;

	UPROPERTY(EditDefaultsOnly, Meta=(BindWidget))
	class UButton* Button_GoCreateRoom;
	
	UPROPERTY(EditDefaultsOnly, Meta=(BindWidget))
	class UButton* Button_GoFindRoom;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UEditableText* Edit_SessionName;

	UFUNCTION()
	void OnMyClickGoCreateRoom();


	UFUNCTION()
	void OnMyClickGoFindRoom();


	// ���ε� ��
	UPROPERTY(EditDefaultsOnly, Meta=(BindWidget))
	class UScrollBox* ScrollBox_RoomList;

	UPROPERTY(EditDefaultsOnly, Meta=(BindWidget))
	class UButton* Button_FindRoom;
	
	UFUNCTION()
	void OnMyClickFindRoom();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URoomInfoUI> roominfoUIFactory;

	UFUNCTION()
	void AddRoomInfoUI(const struct FSessionInfo& info);
	//bool SetSessionName();
};
