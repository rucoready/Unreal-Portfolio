// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMapLodingWidget.h"
#include "Components/ProgressBar.h"

void UMyMapLodingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// �ʱ�ȭ
	progressed = 0.0f;
	elapsedTimes = 0.0f;
	if (loadingMyMap)
	{
		loadingMyMap->SetPercent(progressed);
	};

	// Ÿ�̸� ���� (0.01�ʸ��� ȣ���Ͽ� �ε巯�� ������Ʈ)
	GetWorld()->GetTimerManager().SetTimer(timerHandle_TimerMymapProgressLoading, this, &UMyMapLodingWidget::UpdateMyMapProgressBar, 0.01f, true);
}

void UMyMapLodingWidget::UpdateMyMapProgressBar()
{
	if (loadingMyMap)
	{
		// ���� �ð��� ����Ͽ� ������ ���
		float remainingTime = totalDurations - elapsedTimes;
		float randomFactor = FMath::FRandRange(0.9f, 1.1f); // ���� ��� �߰�
		float increment = (randomFactor * 0.01f) / totalDurations;

		progressed += increment;
		elapsedTimes += 0.01f;

		// �ۼ�Ʈ�� 1�� ���� �ʵ��� �ϰ� �� �ð��� 7�ʸ� ������ Ÿ�̸Ӹ� ������
		if (elapsedTimes >= totalDurations)
		{
			progressed = 1.0f;
			GetWorld()->GetTimerManager().ClearTimer(timerHandle_TimerMymapProgressLoading); // Ÿ�̸� ����
		}

		loadingMyMap->SetPercent(progressed);
	}

}
