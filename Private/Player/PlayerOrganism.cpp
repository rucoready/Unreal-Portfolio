// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerOrganism.h"
#include <Engine/DamageEvents.h>
#include <Kismet/KismetSystemLibrary.h>
#include "Animation/AnimMontage.h"
#include "Player/BaseWeapon.h"
#include "Player/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
//#include "LootPanel.h"
#include "Player/inventory/InventoryComponent.h"
#include "Player/Interfaces/InventoryPanel.h"
#include "Player/World/Pickup.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <Animation/AnimInstance.h>
#include <../../../../../../../Source/Runtime/Core/Public/UObject/NameTypes.h>



// Sets default values
APlayerOrganism::APlayerOrganism()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	stateComp = CreateDefaultSubobject<UStateComponent>(TEXT("StateComp"));

	combatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));

	characterName = TEXT("Player");

	CreateInventory();

	MoveSpeed = 1000.0f;
	dashSkillTime = 0.0f;

	CameraMoveSpeed = 5.0f; // ī�޶� �̵� �ӵ�
	CameraMoveTime = 0.5f;  // ī�޶� �̵� �ð� (��)

}

// Called when the game starts or when spawned
void APlayerOrganism::BeginPlay()
{
	Super::BeginPlay();

	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("characterName : %s"), *characterName));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	stateComp->InitStat();
	stateComp->UpdateStat();
	stateComp->dieDelegate.BindUFunction(this, FName("DieFunction"));

	//TargetLoc = GetActorLocation() + GetActorForwardVector() * 700; // ���ϴ� ��ǥ ��ġ ����
	/*MoveToTargetLocation();*/


	

}

// Called every frame
void APlayerOrganism::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SkillZ)
	{
		if (bDash)
		{
			dashSkillTime += DeltaTime;
			if (bfirstDash)
			{
				//firstDashLoc = GetActorLocation() + ((GetActorForwardVector() + GetActorRightVector() * 0.5f) * 500);
				NewLocation = FMath::Lerp(VS, firstDashLoc, FMath::Clamp(dashSkillTime*InterpSpeed, 0.0f, 1.0f));
				SetActorLocation(NewLocation);
			}
			else if (bsecendDash)
			{
				//secendDashLoc = GetActorLocation() + (GetActorForwardVector() * 500);
				NewLocation = FMath::Lerp(firstDashLoc, secendDashLoc, FMath::Clamp(dashSkillTime*InterpSpeed, 0.0f, 1.0f));
				SetActorLocation(NewLocation);
			}

			if (dashSkillTime >= 0.6f)
			{
				bDash = false;
				bfirstDash = false;
				bsecendDash = false;
				SkillZTarget();
				dashSkillTime = 0.0f;
			}
		}
	}

	if (SkillE)
	{
		// ���� ������ ����Ͽ� ĳ���� ��ġ ������Ʈ
		CurrentLocation = GetActorLocation();
		NewLocation = FMath::Lerp(CurrentLocation, TargetLoc, dashSkillTime);
		SetActorLocation(NewLocation);

		// �̵� �ӵ��� ��Ÿ Ÿ���� ������� Lerp ���� ���� ����
		float LerpDelta = MoveSpeed * DeltaTime / FVector::Dist(CurrentLocation, TargetLoc);
		dashSkillTime = FMath::Clamp(dashSkillTime + LerpDelta, 0.0f, 1.0f);

		


		// ��ǥ ��ġ�� �����ߴ��� Ȯ��
		if (dashSkillTime >= 1.0f)
		{
			dashSkillTime = 0.0f;
			SkillE = false;
		}
	}

	if (bCounterCameraShake)
	{
		APlayerController* APc = Cast<APlayerController>(this->GetController());
		if (APc != nullptr)
		{
			APc->ClientStartCameraShake(PlayerHitShake_bp);
		}
		bCounterCameraShake = false;
	}

	if (bSwordQSkillCameraShake)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PlayerShake"));
		APlayerController* APc = Cast<APlayerController>(this->GetController());
		if (APc != nullptr)
		{
			//UE_LOG(LogTemp,Warning,TEXT("PlayerShake1"));
			APc->ClientStartCameraShake(PlayerQSkillShake_bp);
		}
		bSwordQSkillCameraShake = false;
	}

}

/*if (SkillE)
{
	ESkillTime += DeltaTime;
	FVector Edistance;
	Edistance = FMath::Lerp(CharLoc,TargetLoc,ESkillTime * 3);

	SetActorLocation(Edistance);
	if (ESkillTime >= 1.0f)
	{
		UE_LOG(LogTemp,Warning,TEXT("ESkill"));
		ESkillTime = 0.0f;
		SkillE = false;
	}
}*/


// Called to bind functionality to input
void APlayerOrganism::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	

}

float APlayerOrganism::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bAttackBlock)
	{
		bAttackBlock = false;
 		APlayerController* APc = Cast<APlayerController>(this->GetController());
 		if (APc != nullptr)
 		{
 			//APc->ClientStartCameraShake(PlayerHitShake_bp);
 			SlowDownTime(0.1f, 0.05f, APc);
 		}
		GetWorldTimerManager().SetTimer(timerhandle_CounterShakeTimer, this, &APlayerOrganism::CounterCameraShake, 0.9, false);
		AnimInsatnce = GetMesh()->GetAnimInstance();
		if (AnimInsatnce && AttackCounter != nullptr)
		{ 
			AnimInsatnce->Montage_Play(AttackCounter);
			
		}
	//UE_LOG(LogTemp,Warning,TEXT("PlayerCounter"));
		return 0.0f;
	}

	float temp = stateComp->AddStatePoint(HP, -DamageAmount);

	

	if (PlayerHitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerHitSound, GetActorLocation());
	}

	if (hitReaction != nullptr)
	{
		
	}
	// �����
	if (0)
	{
		if (EventInstigator != nullptr)
		{
			//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("EventInstigator : %s"), *EventInstigator->GetActorNameOrLabel()));
		}

		if (DamageCauser != nullptr)
		{
			//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Attack By : %s"), *DamageCauser->GetActorNameOrLabel()));
		}
	}
	
	

	return 0.0f;
}


void APlayerOrganism::GetHit(const FVector& ImpactPoint, bool bFallDown)
{
	if (!bUseSkill)
	{ 
		return;
	}
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 8.f, 24, FColor::Orange, false, 5.f);

	const FVector Forward = GetActorForwardVector();

	// ���� ���� ��ġ Z�� ���� �� ���� �������
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	// ��� ���� , 0���� �����°� �����ϱ����� GetSafeNormal() ���
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward�� ToHit������ �� ���ϱ� -> Dot Product
	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	//(��Ÿ)�� ���ڻ���(��ũ - �ڻ���)�� ���Ͽ� ��Ÿ�� ���մϴ�.
	// ��ũ �ڻ��ο� �ڻ��� ��Ÿ����
	double Theta = FMath::Acos(CosTheta);
	// ������ ������ ��ȯ
	// Theta�� �� ������ ������ ������
	Theta = FMath::RadiansToDegrees(Theta);

	// ���� ���Ͱ� �Ʒ��� ����Ű�� ��Ÿ ����Ʈ�� ����������,
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
		//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);
	}
		if (hitPaticle != nullptr)
		{
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitPaticle, GetActorLocation() + CrossProduct * 100.f);
		}

		if (bFallDown)
		{
			AnimInsatnce = GetMesh()->GetAnimInstance();
			if (AnimInsatnce && hitFalldownReaction != nullptr)
			{
				if (Theta >= -45.f && Theta < 45.f)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Front"));
				}
				else if (Theta >= -135.f && Theta < -45.f)
				{
					SetActorRotation(GetActorRotation()-FRotator(0,-270,0));
					//UE_LOG(LogTemp, Warning, TEXT("Left"));
				}
				else if (Theta >= 45.f && Theta < 135.f)
				{
					SetActorRotation(GetActorRotation() - FRotator(0,-90,0));
					//UE_LOG(LogTemp, Warning, TEXT("Right"));
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("Back"));
					SetActorRotation(GetActorRotation() - FRotator(0, -180, 0));
				}
				//if (GEngine)
				//{
				//	// ȭ�� ����� ���
				//	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
				//}
				
				AnimInsatnce->Montage_Play(hitFalldownReaction);
				return;
			}
		}
		else
		{ 

		FName Section("HitBack");

		if (Theta >= -45.f && Theta < 45.f)
		{
			Section = FName("HitForward");
			//UE_LOG(LogTemp, Warning, TEXT("Front"));
		}
		else if (Theta >= -135.f && Theta < -45.f)
		{
			Section = FName("HitLeft");
			//UE_LOG(LogTemp, Warning, TEXT("Left"));
		}
		else if (Theta >= 45.f && Theta < 135.f)
		{
			Section = FName("HitRight");
			//UE_LOG(LogTemp, Warning, TEXT("Right"));
		}
		else
		{
		//UE_LOG(LogTemp, Warning, TEXT("Back"));
		}

		PlayHitReactMontage(Section);
		}
	//if (GEngine)
	//{
	//	// ȭ�� ����� ���
	//	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	//}
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.0f, 5.f, FColor::Red, 5.f);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);

}

void APlayerOrganism::PlayHitReactMontage(const FName& SectionName)
{
	AnimInsatnce = GetMesh()->GetAnimInstance();
	if (AnimInsatnce && hitReaction)
	{
		AnimInsatnce->Montage_Play(hitReaction);
		AnimInsatnce->Montage_JumpToSection(SectionName, hitReaction);
	}
}

void APlayerOrganism::ContinueAttack_Implementation()
{
	if (combatComponent == nullptr) return;

	combatComponent->bAttacking = false;

	if (combatComponent->bAttackSaved)
	{
		combatComponent->bAttackSaved = false;

		AttackEvent();
	}
}

void APlayerOrganism::ResetCombat_Implementation()
{
	motionState = ECharacterMotionState::Idle;
	combatComponent->attackCount = 0;
}

bool APlayerOrganism::CanReceiveDamage_Implementation()
{
	return !bDead;
}

void APlayerOrganism::AttackEvent()
{
	if (combatComponent == nullptr) return;

	if (motionState == ECharacterMotionState::Idle || motionState == ECharacterMotionState::Attack)
	{
		PerformAttack(combatComponent->attackCount, false);
	}
}

void APlayerOrganism::PerformAttack(int32 attackIndex, bool bUseRandom) // �̰��� �ε������� �ٲ��ָ� �ִϸ��̼��� �ٸ��� �����. �Ұ��� Ʈ�簡 �Ǹ� ���� ���� �ִϸ��̼� ���.
{
	ABaseWeapon* mainWeapon = nullptr;
	UAnimMontage* useMontage = nullptr;

	if (combatComponent)
	{
		mainWeapon = combatComponent->GetMainWeapon();

		if (mainWeapon != nullptr)
		{
			if (mainWeapon->eWeaponType == EWeaponType::GreatSword) // ����
			{
				useMontage = mainWeapon->attackMontages[0];
			}
			else if (mainWeapon->eWeaponType == EWeaponType::Pick) // ���
			{
				useMontage = mainWeapon->attackMontages[0];
			}
			else
			{
				// ���� �ε��� ����
				int32 montagesSize = mainWeapon->attackMontages.Num();
				int32 randIndex = FMath::RandRange(0, montagesSize - 1);

				useMontage = bUseRandom ? mainWeapon->attackMontages[randIndex] : mainWeapon->attackMontages[attackIndex];
			}
			

			if (IsValid(useMontage))
			{
				ServerRPC_PerformAttack(useMontage);
			}
		}
	}
}

void APlayerOrganism::EnableRagdoll()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None, 0);

		if (GetCapsuleComponent())
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

			FAttachmentTransformRules attachTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);

			if (GetMesh())
			{
				GetMesh()->SetCollisionProfileName(TEXT("ragdoll"));
				GetMesh()->SetAllBodiesBelowSimulatePhysics(pelvisBoneName, true, true);
				GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(pelvisBoneName, 1.0f, false, true);
			}

			//if (CameraBoom)
			//{
			//	if (GetMesh())
			//	{
			//		CameraBoom->AttachToComponent(GetMesh(), attachTransformRules, pelvisBoneName);
			//		CameraBoom->bDoCollisionTest = false;

			//		GetMesh()->SetCollisionProfileName(TEXT("ragdoll"));
			//		GetMesh()->SetAllBodiesBelowSimulatePhysics(pelvisBoneName, true, true);
			//		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(pelvisBoneName, 1.0f, false, true);
			//	}
			//	else
			//	{
			//		UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
			//	}
			//}
			//else
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
			//}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
	}
}

void APlayerOrganism::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerOrganism, stateComp);
	DOREPLIFETIME(APlayerOrganism, combatComponent);
	DOREPLIFETIME(APlayerOrganism, motionState);
	DOREPLIFETIME(APlayerOrganism, bDead);
	//DOREPLIFETIME(APlayerOrganism, itemRandNums);
	//DOREPLIFETIME(APlayerOrganism, randItemIndex);

}

void APlayerOrganism::ServerRPC_PerformAttack_Implementation(UAnimMontage* useMontage)
{
	ABaseWeapon* mainWeapon = combatComponent->GetMainWeapon();

	combatComponent->bAttacking = true;

	motionState = ECharacterMotionState::Attack;

	// ���⳪ �ٸ����� ĳ���� ���� �� ���� ������ �� ����
	mainWeapon->weaponDamage = mainWeapon->weaponDamage;

	// ī��Ʈ ����

	combatComponent->attackCount++;
	if (combatComponent->attackCount >= 5)
	{
		combatComponent->attackCount = 0;
	}

	if (mainWeapon->eWeaponType == EWeaponType::MagicStaff)
	{
		combatComponent->attackCount = 0;
	}

	

	int32 montageLastIndex = mainWeapon->attackMontages.Num() - 1;

	// ī��Ʈ �ʱ�ȭ
	if (combatComponent->attackCount > montageLastIndex)
	{
		combatComponent->attackCount = 0;
	}

	NetMulticastRPC_PerformAttack(useMontage);
}

void APlayerOrganism::NetMulticastRPC_PerformAttack_Implementation(UAnimMontage* useMontage)
{
	float attackAnimTime = PlayAnimMontage(useMontage);

	FTimerHandle handler;
	GetWorldTimerManager().SetTimer(handler, [&]() {

		combatComponent->bAttacking = false;
		motionState = ECharacterMotionState::Idle;

		GetWorldTimerManager().ClearTimer(handler);

		}, 1.0f, false, attackAnimTime);
}

void APlayerOrganism::ServerRPC_AmountDamage_Implementation(float damage)
{
	if (stateComp != nullptr)
	{
		stateComp->AddStatePoint(HP, -damage);
	}
}

void APlayerOrganism::NetMulticastRPC_AmountDamage_Implementation(float damage)
{
	if (stateComp != nullptr)
	{
		stateComp->AddStatePoint(HP, -damage);
	}
}

void APlayerOrganism::DieFunction()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Dead : %s"), *GetActorNameOrLabel()));

	bDead = true;

}

void APlayerOrganism::ServerRPC_PlayerHitSound_Implementation()
{
	NetMulticastRPC_PlayerHitSound();
}

void APlayerOrganism::NetMulticastRPC_PlayerHitSound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerHitSound, GetActorLocation());
}

void APlayerOrganism::SlowDownTime(float DilationAmount, float Duration, APlayerController* PlayerController)
{

	UGameplayStatics::SetGlobalTimeDilation(PlayerController, DilationAmount);

	// ������ �ð� �� �ð� ���� ����
	FTimerHandle Handle;
	PlayerController->GetWorldTimerManager().SetTimer(Handle, [PlayerController]() {
		UGameplayStatics::SetGlobalTimeDilation(PlayerController, 1.0f); // �ð� ���� ����
		}, Duration, false);
	
}

void APlayerOrganism::CounterCameraShake()
{
	APlayerController* APc = Cast<APlayerController>(this->GetController());
	if (APc != nullptr)
	{
		APc->ClientStartCameraShake(PlayerHitShake_bp);
		//SlowDownTime(0.1f, 0.05f, APc);
	}
}



void APlayerOrganism::SkillZTarget()
{
	if (CurrentDashIndex == 2) 
	{
		SetActorLocation(VS);
		SetActorRotation(RS);
		dashSkillTime = 0.0f;
		CurrentDashIndex = 0;
		return;
	}

	bDash = true;

	if (CurrentDashIndex == 0)
	{
		bfirstDash = true;
		firstDashLoc = GetActorLocation() + ((GetActorForwardVector() + GetActorRightVector() * 0.5f) * 500);
		

		//TargetLoc = GetActorLocation() + ((GetActorForwardVector() + GetActorRightVector() * 0.5f) * 500);

		FQuat CurrentQuatRotation = FQuat(GetActorRotation()); // ���� ������Ʈ�� �����̼��� ���ʹϾ����� ��ȯ
		FRotator RelativeRotation(0.f, 250.f, 0.f); // ��������� ȸ���� �� ����

		// ��� ȸ�� ����
		FQuat RelativeQuatRotation = FQuat(RelativeRotation);
		FQuat NewQuatRotation = CurrentQuatRotation * RelativeQuatRotation;

		FRotator NewRotation = NewQuatRotation.Rotator();
		SetActorRotation(NewRotation); // ���ο� ȸ�� ����
		//SetActorLocation(NewLocation);
	}
	else if (CurrentDashIndex == 1)
	{
		bsecendDash = true;
		TargetLoc = GetActorLocation() + (GetActorForwardVector() * 500);
		secendDashLoc = GetActorLocation() + ((GetActorForwardVector() + GetActorRightVector() * 0.5f) * 500);
		

		FQuat CurrentQuatRotation = FQuat(GetActorRotation()); // ���� ������Ʈ�� �����̼��� ���ʹϾ����� ��ȯ
		FRotator RelativeRotation(0.f, -90.0f, 0.f); // ��������� ȸ���� �� ����

		// ��� ȸ�� ����
		FQuat RelativeQuatRotation = FQuat(RelativeRotation);
		FQuat NewQuatRotation = CurrentQuatRotation * RelativeQuatRotation;

		FRotator NewRotation = NewQuatRotation.Rotator();
		SetActorRotation(NewRotation); // ���ο� ȸ�� ����
		//SetActorLocation(NewLocation);
		//SetActorLocation(TargetLoc);
	}

	FVector PlayerLocation = GetActorLocation();
	FString LocationString = PlayerLocation.ToString();
	//UE_LOG(LogTemp, Warning, TEXT("SkillZ: %s"), *LocationString);

	//UE_LOG(LogTemp, Warning, TEXT("SkillZ: %d"), CurrentDashIndex);



	CurrentDashIndex++;
}

void APlayerOrganism::CharcurrentLoc()
{
	//FVector ForwardNomalize = GetActorForwardVector().Normalize();
	if (SkillZ)
	{
		//TargetLoc = ((GetActorForwardVector() + GetActorRightVector()) / 2) * 500;
		//TargetLoc = GetActorLocation();
		VS = GetActorLocation();
		RS = GetActorRotation();
		//CurrentLocation = GetActorLocation();
		//firstDashLoc = GetActorLocation() + ((GetActorForwardVector() + GetActorRightVector() * 0.5f) * 500);
		SkillZTarget();
	
		FVector PlayerLocation = GetActorLocation();
		FString LocationString = PlayerLocation.ToString();
		//UE_LOG(LogTemp, Warning, TEXT("getLoc: %s"), *LocationString);

	}

	//CharLoc = GetActorLocation();
	if (SkillE)
	{ 
		TargetLoc = GetActorLocation() + GetActorForwardVector() * 700;
	}

	//TargetrangeLoc = GetActorLocation() + GetActorForwardVector() * 500;

	
}

//void APlayerOrganism::MoveToTargetLocation()
//{
//	if (!SkillE)
//	{
//		SkillE = true;
//		dashSkillTime = 0.0f; // Lerp ���ĸ� �ʱ�ȭ�Ͽ� ó������ ���� ����
//	}
//}

void APlayerOrganism::LootByOthers(APlayerOrganism* otherCharacter)
{
	//if (motionState != ECharacterMotionState::Die)
	//	return;

	//otherCharacter->GetController();

	//auto panel = Cast<UInventoryPanel>(otherCharacter->lootPanelWidget->GetWidgetFromName(FName(TEXT("WBP_InventoryPanel"))));

	//if (panel)
	//{
	//	panel->SetTargetCharacter(this);
	//	otherCharacter->lootPanelWidget->AddToViewport();
	//}
}

void APlayerOrganism::CreateInventory()
{
	if (PlayerInventory == nullptr)
	{
		PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	}

	PlayerInventory->SetSlotsCapacity(20); //�κ��丮 ���� 20������
	PlayerInventory->SetWeightCapacity(50.0f); // ���Կ뷮 50����
}

//void APlayerOrganism::InitRandomItem()
//{
//	itemRandNums = FMath::RandRange(5, 10);
//
//	ServerRPC_SetItemRandNums(itemRandNums);
//
//	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Init randNums : %d"), itemRandNums));
//	for (int i = 0; i < itemRandNums; i++)
//	{
//		randItemIndex = FMath::RandRange(1, 6);
//
//		ServerRPC_SetItemRandIndex(randItemIndex);
//
//		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Add Item : test_%03d"), randItemIndex));
//
//		ServerRPC_SpawnItem(randItemIndex);
//	}
//
//	PlayerInventory->RefreshInventory();
//}

void APlayerOrganism::ServerRPC_SetItemRandNums_Implementation(int32 randNums)
{
	itemRandNums = randNums;

	NetMulticastRPC_SetItemRandNums(randNums);
}

void APlayerOrganism::NetMulticastRPC_SetItemRandNums_Implementation(int32 randNums)
{
	itemRandNums = randNums;
}

void APlayerOrganism::ServerRPC_SetItemRandIndex_Implementation(int32 randIndex)
{
	randItemIndex = randIndex;

	NetMulticastRPC_SetItemRandIndex(randIndex);
}

void APlayerOrganism::NetMulticastRPC_SetItemRandIndex_Implementation(int32 randIndex)
{
	randItemIndex = randIndex;
}

void APlayerOrganism::ServerRPC_SpawnItem_Implementation(int32 itemIndex)
{
	NetMulticastRPC_SpawnItem(itemIndex);
}

void APlayerOrganism::NetMulticastRPC_SpawnItem_Implementation(int32 randIndex)
{
	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), FTransform(), spawnParam);
	FString strName = FString::Printf(TEXT("test_%03d"), randItemIndex);
	pickup->DesiredItemID = FName(*strName);
	pickup->SetInput(this);
}

