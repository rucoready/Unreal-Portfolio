// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerObjectPoolManager.generated.h"

class APixelCodeCharacter;

UCLASS()
class PIXELCODE_API APlayerObjectPoolManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerObjectPoolManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	TSubclassOf<APixelCodeCharacter> CharacterClass; // Ǯ�� ����� ĳ���� Ŭ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	TArray<APixelCodeCharacter*> Pool; // ĳ���� Ǯ


	APixelCodeCharacter* GetPooledCharacter(); // ĳ���� ��������
	void ReturnPooledCharacter(APixelCodeCharacter* PlayerChar); // ĳ���� ��ȯ�ϱ�


private:
	UPROPERTY(EditAnywhere, Category = "Pool")
	int32 MaxPoolSize; // Ǯ�� �ִ� ũ��

};
