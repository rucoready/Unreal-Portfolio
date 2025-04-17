// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerMageRSkillCastActor.generated.h"

class APlayerMageRSkillSpawnActor;
class USceneComponent;
class ABossApernia;
class ADemonSword;
class AGrux;
class ADogBart;
class USphereComponent;
class UStaticMeshComponent;


UCLASS()
class PIXELCODE_API APlayerMageRSkillCastActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerMageRSkillCastActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnMeteorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<APlayerMageRSkillSpawnActor> ActorToSpawn; // ������ ������ Ŭ���� Ÿ��

	APlayerMageRSkillSpawnActor* RSkillSpawn;

	ABossApernia* boss;
	ADemonSword* demonSword;
	AGrux* grux;
	ADogBart* dogBart;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionSphere; // Sphere �浹 ������Ʈ �߰�

};
