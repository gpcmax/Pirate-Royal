// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum EWeaponEnum
{
	NoWeapon,
	Sword,
	Gun
};

UCLASS()
class COMBATSYSTEM_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	~AMainCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/*Camera Set Up*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* FollowCamera;

	/*Weapon Set up*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapons)
		UStaticMeshComponent* SM_Sword;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapons)
		UStaticMeshComponent* SM_Sheath;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapons)
		UStaticMeshComponent* SM_Gun;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapons)
		USceneComponent* Muzzle;
	
	/*Health System*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		float CurrentHealth;

	/*Attack*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		bool bCanSwing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		bool bIsSwinging;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		bool bIsHeavyAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		bool bIsRolling;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		int SwordAttackNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		int HeavySwordAttackNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		class USoundCue* SwordHitCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		class USoundCue* DamageCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunCombat)
		class USoundCue* RifleFireCue;

	UPROPERTY()
		FTimerHandle SwordTimer;

	UPROPERTY()
		FTimerHandle RollTimer;

	UPROPERTY()
		FTimerHandle ReloadTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		UAnimMontage* LightAttack01;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		UAnimMontage* LightAttack02;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		UAnimMontage* HeavyAttack01;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		UAnimMontage* HeavyAttack02;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		UAnimMontage* RollAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RifleCombat)
		UAnimMontage* EquipGun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RifleCombat)
		UAnimMontage* HitAnim;

	UPROPERTY()
		FTimerHandle EquipGunTimer;

	UPROPERTY()
		FTimerHandle EquipSwordTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RifleCombat)
		UAnimMontage* EquipSword;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RifleCombat)
		UAnimMontage* ReloadAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RifleCombat)
		UAnimMontage* ReloadIronSightsAnim;

/* Death */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Death)
		FVector Respawn01;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Death)
		FVector Respawn02;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Death)
		FVector Respawn03;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Death)
		FVector Respawn04;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Death)
		FVector Respawn05;

/*Sword Collision*/
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		class UBoxComponent* BaseSwordCollision;
		
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		class UBoxComponent* SwordCol01;
		
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		class UBoxComponent* SwordCol02;
		
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		class UBoxComponent* SwordCol03;
		
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		class UBoxComponent* SwordCol04;
		
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = SwordCombat)
		class UBoxComponent* SwordCol05;

	/*Sword Trace*/
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordTrace)
		FVector BaseSwordColVec;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordTrace)
		FVector SwordCol01Vec;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordTrace)
		FVector SwordCol02Vec;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordTrace)
		FVector SwordCol03Vec;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordTrace)
		FVector SwordCol04Vec;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SwordTrace)
		FVector SwordCol05Vec;

	/*Collision*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HitBoxs)
		UCapsuleComponent* HeadCol;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HitBoxs)
		UCapsuleComponent* BodyCol;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HitBoxs)
		UCapsuleComponent* LeftUpperArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HitBoxs)
		UCapsuleComponent* LeftLowerArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HitBoxs)
		UCapsuleComponent* RightUpperArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HitBoxs)
		UCapsuleComponent* RightLowerArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HitBoxs)
		UCapsuleComponent* LeftCalfLeg;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HitBoxs)
		UCapsuleComponent* LeftThighLeg;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HitBoxs)
		UCapsuleComponent* RightCalfLeg;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HitBoxs)
		UCapsuleComponent* RightThighLeg;

	/*bools and int*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapons)
		bool bHoldingGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapons)
		bool bHoldingSword;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapons)
		bool bIsADS;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = LockOnSystem)
		bool bIsLockedOn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = LockOnSystem)
		bool bIsPlayerOne;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapons)
		int CurrentAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapons)
		int MaxAmmo;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Time)
		float DeltaSeconds;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Combat)
		float GuardStanceValue;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Weapons)
		bool Guarding;

	UPROPERTY(BlueprintReadWrite)
		TEnumAsByte<EWeaponEnum> WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LockOnSystem)
		class AMainCharacter* EnemyFound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LockOnSystem)
		class AMainCharacter* EnemyFound02;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveRight(float Value);

	void MoveForward(float Value);

	void BlockAttacks();

	void StopBlock();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	//Function for equiping the rifle
	UFUNCTION(BlueprintCallable)
		void RifleEquip();

	//function of putting rifle back
	UFUNCTION(BlueprintCallable)
		void RifleUnEquip();

	//function for firing the rifle
	UFUNCTION(BlueprintCallable)
		void RifleFire();

	//function for reloading the rifle
	UFUNCTION(BlueprintCallable)
		void RifleReload();

	//function for equipping the sword
	UFUNCTION(BlueprintCallable)
		void SwordEquip();

	//function for putting the sword back
	UFUNCTION(BlueprintCallable)
		void SwordUnequip();

	//put back any weapon being held
	UFUNCTION(BlueprintCallable)
		void UnEquipAll();

	//handles aiming 
	UFUNCTION(BlueprintCallable)
		void Aim();

	//handles stop aiming
	UFUNCTION(BlueprintCallable)
		void StopAiming();

	//handles dark souls lock on
	UFUNCTION(BlueprintCallable)
		void LockOn();

	//handles input for lock on 
	UFUNCTION(BlueprintCallable)
		void LockOnSetter();

	//updates player rotation based off of enemies
	UFUNCTION(BlueprintCallable)
		void UpdatePlayerRot();

	//handles attack
	UFUNCTION(BlueprintCallable)
		void Attack();

	//handles function for light attacks
	UFUNCTION(BlueprintCallable)
		void LightAttack();

	//handles when the player rolls
	UFUNCTION(BlueprintCallable)
		void Roll();
	
	//handles timer resets
	UFUNCTION()
		void TimerReset();

	//reloads guns
	UFUNCTION()
		void AmmoInGun();

	//attaches gun to hand after timer
	UFUNCTION()
		void AttachGunToHand();

	//attaches sword to hand after timer
	UFUNCTION()
		void AttachSwordToHand();

	//UFUNCTION()
		//virtual void BeginDestroy();
	
};
