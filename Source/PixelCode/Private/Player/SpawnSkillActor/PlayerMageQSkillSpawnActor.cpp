// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpawnSkillActor/PlayerMageQSkillSpawnActor.h"
#include "Boss/BossApernia.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include "DemonSword.h"
#include "Grux.h"
#include "DogBart.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Particles/ParticleSystem.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Components/BoxComponent.h"


// Sets default values
APlayerMageQSkillSpawnActor::APlayerMageQSkillSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);

	//boxComp2 = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp2"));
	//boxComp2->SetupAttachment(RootComponent);

	//boxComp3 = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp3"));
	//boxComp3->SetupAttachment(RootComponent);

	boxComp->SetGenerateOverlapEvents(true);

	NA_MageQSkillComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	NA_MageQSkillComp->SetupAttachment(RootComponent);

	// �ʱ� �� ����
	//InitialScale = FVector(4.f, 6.f, 2.3f);
	//boxComp->SetWorldScale3D(InitialScale);

	// �ʱ� �� ����
	InitialScale = FVector(4.f, 8.f, 2.3f);
	boxComp->SetWorldScale3D(InitialScale);

	ElapsedTime = 0.f; // ��� �ð� �ʱ�ȭ
	MaxScaleFactor = 3.f; // ������ ���ϴ� �ִ� ������ ����
	ScaleSpeed = 2.0f; // ������ ��ȭ �ӵ�
}

// Called when the game starts or when spawned
void APlayerMageQSkillSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * 100);

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerMageQSkillSpawnActor::OnOverlapEnemy);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageQSkill, GetActorLocation(), GetActorRotation());

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerMageQskillSound, GetActorLocation());

}

// Called every frame
void APlayerMageQSkillSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ��� �ð� ������Ʈ
	ElapsedTime += DeltaTime;

	// �ð��� ���� ������ ���� ��� (�����θ� Ŀ���� ȿ��)
	float ScaleFactor = 1.f + FMath::Sin(ElapsedTime * ScaleSpeed) * (MaxScaleFactor - 1.f);

	// ���� �������� �ڽ� ������Ʈ�� ����
	FVector NewScale = InitialScale * ScaleFactor;
	boxComp->SetWorldScale3D(NewScale);

	if (ElapsedTime >= 2.4f)
	{
		Destroy();
	}

}

void APlayerMageQSkillSpawnActor::OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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



	UParticleSystemComponent* SpawnedParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageQSkillHit, GetActorLocation(), GetActorRotation());
	
	

}

