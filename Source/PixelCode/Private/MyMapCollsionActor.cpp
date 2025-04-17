// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMapCollsionActor.h"
#include "PCodePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"

// Sets default values
AMyMapCollsionActor::AMyMapCollsionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxMapCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollision"));
}

// Called when the game starts or when spawned
void AMyMapCollsionActor::BeginPlay()
{
	Super::BeginPlay();
	boxMapCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyMapCollsionActor::OnBeginOverlapMapPortal);
	
}

// Called every frame
void AMyMapCollsionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyMapCollsionActor::OnBeginOverlapMapPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		APawn* OverlappedPawn = Cast<APawn>(OtherActor);
		if (OverlappedPawn)
		{
			// APCodePlayerController Ÿ������ ĳ����
			APCodePlayerController* OverlappedPlayerController = Cast<APCodePlayerController>(OverlappedPawn->GetController());
			if (OverlappedPlayerController)
			{
				OverlappedPlayerController->CreateWidgetMyMAPs();
				UE_LOG(LogTemp, Warning,TEXT("adasdasdasda"));
			}
		}
	}
}

void AMyMapCollsionActor::ServerRPC_LevelMoveMap_Implementation()
{
	MulticastRPC_LevelMoveMap();
	GetWorld()->ServerTravel(TEXT("/Game/KSH/Maps/Map?Listen"));
}

void AMyMapCollsionActor::MulticastRPC_LevelMoveMap_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		// PCodePlayerController Ÿ������ ĳ����
		APCodePlayerController* PCodePlayerController = Cast<APCodePlayerController>(It->Get());
		if (PCodePlayerController)
		{
			PCodePlayerController->ServerRPC_CreateWidgetMyMap();
			PCodePlayerController->ClientTravel("/Game/KSH/Maps/Map?Listen", TRAVEL_Absolute);
			//tWorld()->GetTimerManager().SetTimer(handle, this, &APCodePlayerController::ServerRPC_RespawnPlayer_Implementation, 5, false);
		}
	}

}

void AMyMapCollsionActor::RemoveMapLoadingUI()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		// PCodePlayerController Ÿ������ ĳ����
		APCodePlayerController* PCodePlayerController = Cast<APCodePlayerController>(It->Get());
		if (PCodePlayerController)
		{
			PCodePlayerController->ServerRPC_HideWidgetMyMap();
		}
	}
}

