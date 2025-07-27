// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatInterface.h"
#include "StateComponent.h"
#include "inventory/InventoryComponent.h"
#include "PlayerOrganism.generated.h"

UENUM()
enum class ECharacterType : int8
{
	PlayerCharacter,
	Enemy
};

UENUM()
enum class ECharacterMotionState : int8
{
	Idle,
	ToggleCombat,
	Attack,
	Defence,
	//Hit,
	Jump,
	Die
};

class UAnimMontage;
class UCombatComponent;
class ULootPanel;
UCLASS()
class PIXELCODE_API APlayerOrganism : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerOrganism();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString characterName;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool bDead;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	UStateComponent* stateComp;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCombatComponent* combatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	ECharacterType characterType;
	
	// �ǰݽ� HitReactMontage ����� ���� �Լ�
	void PlayHitReactMontage(const FName& SectionName);

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	UAnimMontage* hitReaction;	

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	UAnimMontage* hitFalldownReaction;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	UAnimMontage* AttackCounter;

	class UAnimInstance* AnimInsatnce;

	void GetHit(const FVector& ImpactPoint, bool bFallDown);

	UPROPERTY(Replicated, BlueprintReadWrite)
	ECharacterMotionState motionState = ECharacterMotionState::Idle;

	
	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	virtual void ContinueAttack_Implementation() override;
	virtual void ResetCombat_Implementation() override;
	virtual bool CanReceiveDamage_Implementation() override;

	TArray<int32> attackDamageArray;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)override;

	
	FName pelvisBoneName = TEXT("Pelvis");

	UFUNCTION(BlueprintCallable)
	void AttackEvent();

	UFUNCTION(BlueprintCallable)
	void PerformAttack(int32 attackIndex, bool bUseRandom);

	void EnableRagdoll();

	FString GetName() {return characterName;};

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_PerformAttack(UAnimMontage* useMontage);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PerformAttack(UAnimMontage* useMontage);

	UFUNCTION(Server, Reliable)
	void ServerRPC_AmountDamage(float damage);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_AmountDamage(float currentHP);

	UFUNCTION()
	virtual void DieFunction();

	bool bFarmFoliage = false;
	bool bMine = false;

	bool bBindEnemy = false;

	bool SkillZ = false;

	// ĳ���Ͱ� ���� �̵� ������ ����
	bool SkillE = false;

	// Lerp ���� ���� (���� ����)
	float dashSkillTime = 0.0f;

	// �÷��̾� ������ ȸ�� ����
	bool bRotation = false;

	// �÷��̾� ��ų�� �ٸ����� ���ϰ� ����
	bool bUseSkill = false;
	
	// �÷��̾� ��ų ���̾ư���
	bool bSkillNSQ = false;
	bool bSkillNSR = false;

	bool bBossGroggy = false;

	// ī���� ����
	bool bAttackBlock = false;

	// ī�޶� ����ũ
	bool bSwordQSkillCameraShake = false;

	// �÷��̾� ����
	bool bSwordInSound = false;

	bool bSwordOutSound = false;

	bool bBaseSwordSound1 = false;

	bool bBaseSwordSound2 = false;

	bool bBaseSwordSound3 = false;

	bool bBaseSwordSound4 = false;

	bool bBaseSwordSound5 = false;

	bool bRightSwordSound1 = false;

	bool bRightSwordSound2 = false;

	bool bSwordQ1Sound = false;

	bool bSwordQ2Sound = false;

	bool bSwordQ3Sound = false;

	bool bSwordQ4Sound = false;

	bool bSwordQ5Sound = false;

	bool bSwordE1Sound = false;
	
	bool bSwordE2Sound = false;

	bool bSwordE3Sound = false;

	bool bSwordE4Sound = false;

	bool bSwordE5Sound = false;

	bool bSwordR1Sound = false;

	bool bSwordZ1Sound = false;

	bool bSwordZ2Sound = false;

	bool bSwordZ3Sound = false;
	
	bool bSwordZ4Sound = false;

	bool bSwordZ5Sound = false;

	bool bmageRightAttack = false;

	bool bmageQAttack = false;

	bool bmageEAttack = false;

	bool bmageRAttack = false;

	bool bMove = false;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerHitSound;

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerHitSound();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerHitSound();


	void SlowDownTime(float DilationAmount, float Duration, APlayerController* PlayerController);

	UPROPERTY(EditAnywhere,Category = "MySettings|Varibles")
	TSubclassOf<class UCameraShakeBase> PlayerHitShake_bp;

	UPROPERTY(EditAnywhere,Category = "MySettings|Varibles")
	TSubclassOf<class UCameraShakeBase> PlayerQSkillShake_bp;

	bool bCounterCameraShake = false;


	FTimerHandle timerhandle_CounterShakeTimer;

	void CounterCameraShake();

	bool bIsJump = false;

	UPROPERTY(EditAnywhere)
	float InterpSpeed = 10.0f;

	 // ��ǥ ��ġ ����
    UPROPERTY(EditAnywhere, Category = "Movement")
    FVector TargetLoc;

	// �ﰢ�� �׽�Ʈ

	float ZSkillTimer = 0.0f;

	bool bDash = false;

	bool bfirstDash = false;
	bool bsecendDash = false;


    int32 CurrentDashIndex = 0;

    FTimerHandle DashTimerHandle;

	FVector VS;
	FRotator RS;

	FVector CurrentLocation;

	FVector NewLocation;

	FVector firstDashLoc;
	FVector secendDashLoc;

   void SkillZTarget();

   
	
	

	// ������� �׽�Ʈ


	UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed;

	// ���� ������ ����Ͽ� ĳ���͸� ��ǥ ��ġ�� �̵���Ű�� �Լ�
	//void MoveToTargetLocation();

	// ĳ���� ��ġ����
	void CharcurrentLoc();


    // ī�޶� �̵� �ӵ� ����
    float CameraMoveSpeed;

    // ī�޶� �̵� �ð� ����
    float CameraMoveTime;


	
	/*UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<ULootPanel> lootPanelWidgetClass;
		
	UPROPERTY()
	ULootPanel* lootPanelWidget;	 */

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; }; // �κ��丮 ��������

	void LootByOthers(APlayerOrganism* otherCharacter);

	virtual void CreateInventory();

	/*void InitRandomItem();*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* hitPaticle;

	UPROPERTY()
	int32 itemRandNums = 0;

	UPROPERTY()
	int32 randItemIndex = 0;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetItemRandNums(int32 randNums);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_SetItemRandNums(int32 randNums);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetItemRandIndex(int32 randIndex);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_SetItemRandIndex(int32 randIndex);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnItem(int32 itemIndex);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_SpawnItem(int32 randIndex);
};