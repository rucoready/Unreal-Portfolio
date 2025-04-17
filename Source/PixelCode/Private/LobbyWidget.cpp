// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Slider.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/EditableText.h>
#include "PCodeGameInstance.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetSwitcher.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ScrollBox.h>
#include "RoomInfoUI.h"


void ULobbyWidget::NativeConstruct()
{
	// ���� �ν��Ͻ� ä���
	gi = Cast<UPCodeGameInstance>(GetWorld()->GetGameInstance());


	gi->OnMySessionSearchCompleteDelegate.AddDynamic(this, &ULobbyWidget::AddRoomInfoUI);
	

	// ��ư
	Button_CreatRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickCreateRoom);
	Button_GoCreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoCreateRoom);
	Button_GoFindRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoFindRoom);
	Button_FindRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickFindRoom);
	

	Button_Back1->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickBackRoom);
	Button_Back2->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickBackRoom);
	
	SwitcherUI->SetActiveWidgetIndex(0);

	Slider_PlayerCount->SetValue(FCString::Atof(*Text_PlayerCount->GetText().ToString()));

	Slider_PlayerCount->OnValueChanged.AddDynamic(this, &ULobbyWidget::OnValueChanged);

	SwitcherUI->SetActiveWidgetIndex(0);
}

void ULobbyWidget::OnMyClickBackRoom()
{
	SwitcherUI->SetActiveWidgetIndex(0);
}

void ULobbyWidget::OnMyClickGoCreateRoom()
{
	SwitcherUI->SetActiveWidgetIndex(1);
}

void ULobbyWidget::OnMyClickGoFindRoom()
{
	/*if (false == SetSessionName())
		return;*/

	SwitcherUI->SetActiveWidgetIndex(2);
	// ��ã�� �޴� ���Խ� ã�⸦ �õ��ϰ�ʹ�.
	if (gi)
	{
		gi->FindOtherSessions();
	}
}



void ULobbyWidget::AddRoomInfoUI(const struct FSessionInfo& info)
{
	// RoomInfoUIFactory�� �̿��ؼ� ������ �����
	auto ui = CreateWidget<URoomInfoUI>(this, roominfoUIFactory);
	// info�� Setup�Լ��� ���� �����ϰ�ʹ�.
	ui->Setup(info);
	// ������ ������ ScrollBox_RoomList�� ���̰�ʹ�.
	ScrollBox_RoomList->AddChild(ui);
}

//bool ULobbyWidget::SetSessionName()
//{
//	// �ؾ�����!
//		// badword üũ!
//	FString str = Edit_SessionName->GetText().ToString();
//	//str = str.TrimStartAndEnd();
//	//str = str.Replace(TEXT("|"), TEXT(""));
//	//str.Split()
//	//str.Replace()
//	// �Է��� ���� ���ٸ�
//	if (str.IsEmpty()) {
//		return false;
//	}
//}

void ULobbyWidget::OnMyClickCreateRoom()
{
	if(gi)
	{
		int32 count = Slider_PlayerCount->GetValue();
		gi->CreateMySession(Edit_RoomName->GetText().ToString(),count);
	}
}

void ULobbyWidget::OnMyClickFindRoom()
{
	// ������ ����� �����ϰ�ʹ�.
	ScrollBox_RoomList->ClearChildren();
	// gi�� FindOtherSessions�� ȣ���ϰ�ʹ�.
	gi->FindOtherSessions();
}

void ULobbyWidget::OnValueChanged(float value)
{
	Text_PlayerCount->SetText(FText::AsNumber(value));
}