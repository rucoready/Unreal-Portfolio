// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpawnSkillActor/SpawnSwordRSkill.h"
#include "Components/BoxComponent.h"
#include "Boss/BossApernia.h"
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include "DemonSword.h"
#include "Grux.h"
#include "DogBart.h"

// Sets default values
ASpawnSwordRSkill::ASpawnSwordRSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);

	boxComp->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void ASpawnSwordRSkill::BeginPlay()
{
	Super::BeginPlay();

	// �ʱ� ��ġ ����
	SetActorLocation(GetActorLocation() + GetActorForwardVector()*300); // ���÷� ���͸� Z������ 100��ŭ �̵���Ŵ
	bDestroy = true;

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ASpawnSwordRSkill::OnOverlapEnemy);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_SkillR, GetActorLocation(), GetActorRotation());
	
}

// Called every frame
void ASpawnSwordRSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime; // Speed�� ������ �̵��� �ӵ�
	SetActorLocation(NewLocation);
	
	if (bDestroy)
	{
		DestroyTime += DeltaTime;
		if (DestroyTime >= 1.0f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("destroy"));
			bDestroy = false;
			DestroyTime = 0.0f;
			Destroy();
		}
	}
}

void ASpawnSwordRSkill::OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// OtherActor�� ���ʹ��̹Ƿ� �������� ����
	ABossApernia* boss = Cast<ABossApernia>(OtherActor);
	ADemonSword* demonSword = Cast<ADemonSword>(OtherActor);
	AGrux* grux = Cast<AGrux>(OtherActor);
	ADogBart* dogBart = Cast<ADogBart>(OtherActor);
	if (boss)
	{
		// ������ ���� ����: TakeDamage �Լ� ȣ��
		boss->BossTakeDamage(DamageAmount);
	}
	else if (demonSword)
	{
		demonSword->SwordTakeDamage(DamageAmount);
	}
	else if (grux)
	{
		grux->GruxTakeDamage(DamageAmount);
	}
	else if (dogBart)
	{
		dogBart->DogBartTakeDamage(DamageAmount);
	}
}

