// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpawnSkillActor/PlayerMageRSkillCastActor.h"
#include "Boss/BossApernia.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include "DemonSword.h"
#include "Grux.h"
#include "DogBart.h"
#include "BoundCollision.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h>
#include "PCodePlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Player/SpawnSkillActor/PlayerMageRSkillSpawnActor.h"
#include "Components/SphereComponent.h"

// Sets default values
APlayerMageRSkillCastActor::APlayerMageRSkillCastActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


    // Sphere �ø����� ���� ������Ʈ ����
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->InitSphereRadius(100.0f); // ���� ������ ���� (������ ũ��� ���� �ʿ�)

    CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionSphere->SetSimulatePhysics(true);
    RootComponent = CollisionSphere;

}

// Called when the game starts or when spawned
void APlayerMageRSkillCastActor::BeginPlay()
{
	Super::BeginPlay();
	
    SetActorLocation(GetActorLocation() + FVector(0, 0, 1000));
    SetActorRotation(GetActorRotation());

    FVector LaunchDirection = GetActorRotation().Vector();
    CollisionSphere->AddImpulse(LaunchDirection * 2000);

    //NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Meteor, GetActorLocation());
   

    // OnActorHit �̺�Ʈ�� �ڵ鷯 �Լ��� ���ε�
    OnActorHit.AddDynamic(this, &APlayerMageRSkillCastActor::OnMeteorHit);


   

}

// Called every frame
void APlayerMageRSkillCastActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerMageRSkillCastActor::OnMeteorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != this)
    {
        boss = Cast<ABossApernia>(OtherActor);
        demonSword = Cast<ADemonSword>(OtherActor);
        grux = Cast<AGrux>(OtherActor);
        dogBart = Cast<ADogBart>(OtherActor);

        //.Add �޼��带 ����ϸ� �������� �迭�� ũ�� ����, �ʱ⿡ �迭�� ũ�⸦ ������ �ʰ� �ʿ��� ������ ��Ҹ� �߰� ����.
         // ���� �����Ǹ� �ش� ���� �迭�� �߰��ϰ� ������ ���� Ÿ�̸Ӹ� �����մϴ�.
       
            // �浹 ������ ������ ���͸� ����
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            APlayerMageRSkillSpawnActor* SpawnedActor = GetWorld()->SpawnActor<APlayerMageRSkillSpawnActor>(ActorToSpawn, Hit.ImpactPoint, FRotator::ZeroRotator, SpawnParams);

            if (SpawnedActor)
            {
                Destroy();
            }
        
       
    }
    
     
}




