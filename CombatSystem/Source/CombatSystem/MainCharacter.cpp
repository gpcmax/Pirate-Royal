// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "VillianPlayer.h"
#include "Engine.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	//spring arm component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 100.f);
	CameraBoom->bUsePawnControlRotation = true;

//camera component
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

/*weapons*/
	SM_Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Sword"));
	SM_Sword->SetupAttachment(GetMesh());

	SM_Sheath = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Sheath"));
	SM_Sheath->SetupAttachment(GetMesh());

	SM_Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Gun"));
	SM_Gun->SetupAttachment(GetMesh());

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(SM_Gun);

/*Sword Collision*/
	
	BaseSwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BaseSwordCollision"));
	BaseSwordCollision->SetupAttachment(SM_Sword);
	
	SwordCol01 = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCol01"));
	SwordCol01->SetupAttachment(BaseSwordCollision);
	
	SwordCol02 = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCol02"));
	SwordCol02->SetupAttachment(BaseSwordCollision);
	
	SwordCol03 = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCol03"));
	SwordCol03->SetupAttachment(BaseSwordCollision);
	
	SwordCol04 = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCol04"));
	SwordCol04->SetupAttachment(BaseSwordCollision);
	
	SwordCol05 = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCol05"));
	SwordCol05->SetupAttachment(BaseSwordCollision);
	


/*collision for body hits*/
	HeadCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadCol"));
	HeadCol->SetupAttachment(GetMesh(), "head");
	HeadCol->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	BodyCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCol"));
	BodyCol->SetupAttachment(GetMesh(), "spine_02");
	BodyCol->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	LeftLowerArm = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftLowerArm"));
	LeftLowerArm->SetupAttachment(GetMesh(), "lowerarm_l");
	LeftLowerArm->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	RightLowerArm = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightLowerArm"));
	RightLowerArm->SetupAttachment(GetMesh(), "lowerarm_r");
	RightLowerArm->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	RightUpperArm = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightUpperArm"));
	RightUpperArm->SetupAttachment(GetMesh(), "upperarm_r");
	RightUpperArm->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	LeftUpperArm = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftUpperArm"));
	LeftUpperArm->SetupAttachment(GetMesh(), "upperarm_l");
	LeftUpperArm->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	LeftCalfLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftCalfLeg"));
	LeftCalfLeg->SetupAttachment(GetMesh(), "calf_l");
	LeftCalfLeg->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	RightCalfLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightCalfLeg"));
	RightCalfLeg->SetupAttachment(GetMesh(), "calf_r");
	RightCalfLeg->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	RightThighLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightThighLeg"));
	RightThighLeg->SetupAttachment(GetMesh(), "thigh_r");
	RightThighLeg->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	LeftThighLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftThighLeg"));
	LeftThighLeg->SetupAttachment(GetMesh(), "thigh_l");
	LeftThighLeg->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

/*Default Variables*/
	bHoldingGun = false;
	bHoldingSword = false;
	bIsADS = false;
	bIsLockedOn = false;
	Guarding = false;
	//bIsPlayerOne = true;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	MaxHealth = 100.f;

	MaxAmmo = 1;
}

AMainCharacter::~AMainCharacter()
{
	if (bIsLockedOn)
	{
		LockOnSetter();
	}
	
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAmmo = MaxAmmo;

	CurrentHealth = MaxHealth;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeltaSeconds = DeltaTime;

	if (CurrentHealth <= 0)
	{
		if (bIsLockedOn)
		{
			LockOnSetter();
		}
		Destroy(this);
	}
	
	/*Swing trace*/
	if (bIsSwinging)
	{
		///////////////////////////////////////////////////////////
		/*BaseSword Variables*/
		FHitResult BaseSwordhit;
		FColor traceColor = FColor::Red;
		FVector BaseSwordStartVector = BaseSwordCollision->GetComponentLocation();
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);

		/*Sword01 Variables*/
		FHitResult SwordCol01Hit;
		FVector SwordCol01StartVector = SwordCol01->GetComponentLocation();

		/*Sword02 Variables*/
		FHitResult SwordCol02Hit;
		FVector SwordCol02StartVector = SwordCol02->GetComponentLocation();

		/*Sword 03 Variables*/
		FHitResult SwordCol03Hit;
		FVector SwordCol03StartVector = SwordCol03->GetComponentLocation();

		/*Sword 04 Variables*/
		FHitResult SwordCol04Hit;
		FVector SwordCol04StartVector = SwordCol04->GetComponentLocation();

		/*Sword 05 Variables*/
		FHitResult SwordCol05Hit;
		FVector SwordCol05StartVector = SwordCol05->GetComponentLocation();

		//Base Sword Trace
		GetWorld()->LineTraceSingleByChannel(BaseSwordhit, BaseSwordStartVector, BaseSwordColVec, ECC_Visibility, TraceParams);
		//Sword01 Trace
		GetWorld()->LineTraceSingleByChannel(SwordCol01Hit, SwordCol01StartVector, SwordCol01Vec, ECC_Visibility, TraceParams);
		//Sword02 Trace
		GetWorld()->LineTraceSingleByChannel(SwordCol02Hit, SwordCol02StartVector, SwordCol02Vec, ECC_Visibility, TraceParams);
		//Sword03 Trace
		GetWorld()->LineTraceSingleByChannel(SwordCol03Hit, SwordCol03StartVector, SwordCol03Vec, ECC_Visibility, TraceParams);
		//Sword 04 trace
		GetWorld()->LineTraceSingleByChannel(SwordCol04Hit, SwordCol04StartVector, SwordCol04Vec, ECC_Visibility, TraceParams);
		//Sword 05 trace
		GetWorld()->LineTraceSingleByChannel(SwordCol05Hit, SwordCol05StartVector, SwordCol05Vec, ECC_Visibility, TraceParams);

		if (BaseSwordhit.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit"));
			traceColor = FColor::Blue;
			UGameplayStatics::ApplyPointDamage(BaseSwordhit.GetActor(), 10.f, BaseSwordhit.ImpactPoint, BaseSwordhit, NULL, this, NULL);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwordHitCue, BaseSwordhit.ImpactPoint, 1.5f, 1.f, 0.0f, nullptr, nullptr);
			bIsSwinging = false;
		}
		else if (SwordCol01Hit.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Sword Col 01 Hit"));
			traceColor = FColor::Blue;
			UGameplayStatics::ApplyPointDamage(SwordCol01Hit.GetActor(), 10.f, SwordCol01Hit.ImpactPoint, SwordCol01Hit, NULL, this, NULL);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwordHitCue, SwordCol01Hit.ImpactPoint, 1.5f, 1.f, 0.0f, nullptr, nullptr);
			bIsSwinging = false;
		}
		else if (SwordCol02Hit.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Sword Col 02 Hit"));
			traceColor = FColor::Blue;
			UGameplayStatics::ApplyPointDamage(SwordCol02Hit.GetActor(), 10.f, SwordCol02Hit.ImpactPoint, SwordCol02Hit, NULL, this, NULL);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwordHitCue, SwordCol02Hit.ImpactPoint, 1.5f, 1.f, 0.0f, nullptr, nullptr);
			bIsSwinging = false;
		}
		else if (SwordCol03Hit.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Sword Col 03 Hit"));
			traceColor = FColor::Blue;
			UGameplayStatics::ApplyPointDamage(SwordCol03Hit.GetActor(), 10.f, SwordCol03Hit.ImpactPoint, SwordCol03Hit, NULL, this, NULL);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwordHitCue, SwordCol03Hit.ImpactPoint, 1.5f, 1.f, 0.0f, nullptr, nullptr);
			bIsSwinging = false;
		}
		else if (SwordCol04Hit.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Sword Col 04 Hit"));
			traceColor = FColor::Blue;
			UGameplayStatics::ApplyPointDamage(SwordCol04Hit.GetActor(), 10.f, SwordCol04Hit.ImpactPoint, SwordCol04Hit, NULL, this, NULL);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwordHitCue, SwordCol04Hit.ImpactPoint, 1.5f, 1.f, 0.0f, nullptr, nullptr);
			bIsSwinging = false;
		}
		else if (SwordCol05Hit.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Sword Col 05 Hit"));
			traceColor = FColor::Blue;
			UGameplayStatics::ApplyPointDamage(SwordCol05Hit.GetActor(), 10.f, SwordCol05Hit.ImpactPoint, SwordCol05Hit, NULL, this, NULL);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwordHitCue, SwordCol05Hit.ImpactPoint, 1.5f, 1.f, 0.0f, nullptr, nullptr);
			bIsSwinging = false;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NotHit"));
			traceColor = FColor::Red;
			//bIsSwinging = false;
		}
		/*
		//adds debug lines to sword traces
		DrawDebugLine(GetWorld(), BaseSwordStartVector, BaseSwordColVec, traceColor, false, 5.f, 0);
		DrawDebugLine(GetWorld(), SwordCol01StartVector, SwordCol01Vec, traceColor, false, 5.f, 0);
		DrawDebugLine(GetWorld(), SwordCol02StartVector, SwordCol02Vec, traceColor, false, 5.f, 0);
		DrawDebugLine(GetWorld(), SwordCol03StartVector, SwordCol03Vec, traceColor, false, 5.f, 0);
		DrawDebugLine(GetWorld(), SwordCol04StartVector, SwordCol04Vec, traceColor, false, 5.f, 0);
		DrawDebugLine(GetWorld(), SwordCol05StartVector, SwordCol05Vec, traceColor, false, 5.f, 0);
		*/
	}

	BaseSwordColVec = BaseSwordCollision->GetComponentLocation();
	SwordCol01Vec = SwordCol01->GetComponentLocation();
	SwordCol02Vec = SwordCol02->GetComponentLocation();
	SwordCol03Vec = SwordCol03->GetComponentLocation();
	SwordCol04Vec = SwordCol04->GetComponentLocation();
	SwordCol05Vec = SwordCol05->GetComponentLocation();

	if (bIsLockedOn)
	{
		//once player is locked on the players rotation is updated to track the enemies
		UpdatePlayerRot();
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//input axis
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacter::LookUpAtRate);

	//input action
	PlayerInputComponent->BindAction("RifleEquipInput", IE_Pressed, this, &AMainCharacter::RifleEquip);
	PlayerInputComponent->BindAction("SwordEquipInput", IE_Pressed, this, &AMainCharacter::SwordEquip);
	PlayerInputComponent->BindAction("UnEquipAll", IE_Pressed, this, &AMainCharacter::UnEquipAll);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::Aim);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacter::RifleReload);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMainCharacter::StopAiming);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacter::Attack);
	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &AMainCharacter::LightAttack);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMainCharacter::Roll);
	PlayerInputComponent->BindAction("LockOnto", IE_Pressed, this, &AMainCharacter::LockOnSetter);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AMainCharacter::BlockAttacks);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &AMainCharacter::StopBlock);
}

void AMainCharacter::BlockAttacks()
{
	Guarding = true;
	GuardStanceValue = 1.f;
}

void AMainCharacter::StopBlock()
{
	Guarding = false;
	GuardStanceValue = 0.f;
}

/*Controller turn rate*/
void AMainCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate* BaseTurnRate* GetWorld()->GetDeltaSeconds());
}

/*Controller look around rate*/
void AMainCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate*BaseLookUpRate*GetWorld()->GetDeltaSeconds());
}

/*Code to move the player for both controller and keyboard mouse*/
void AMainCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

/*Code to move the player for both controller and keyboard mouse*/
void AMainCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

/*Handles when the player takes damage and checks where they got hit and can apply different damage depending on where*/
float AMainCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	FHitResult HitResult;
	FVector impluseDir;
	DamageEvent.GetBestHitInfo(this, DamageCauser, HitResult, impluseDir);
	impluseDir.Normalize();

	if (!Guarding)
	{
		if (HitResult.GetComponent() == HeadCol)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Boom Headshot")));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageCue, HeadCol->GetSocketLocation("None"), 3.f, 1.f, 0.0f, nullptr, nullptr);
			PlayAnimMontage(HitAnim);
			CurrentHealth = (CurrentHealth - Damage) - 40;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHealth));
		}
		else if (HitResult.GetComponent() == BodyCol)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("My Chest!")));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageCue, BodyCol->GetSocketLocation("None"), 3.f, 1.f, 0.0f, nullptr, nullptr);
			PlayAnimMontage(HitAnim);
			CurrentHealth = CurrentHealth - Damage - 20;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHealth));
		}
		else if (HitResult.GetComponent() == LeftLowerArm)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("My Lower Left arm!")));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageCue, LeftLowerArm->GetSocketLocation("None"), 3.f, 1.f, 0.0f, nullptr, nullptr);
			PlayAnimMontage(HitAnim);
			CurrentHealth = CurrentHealth - Damage;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHealth));
		}
		else if (HitResult.GetComponent() == RightLowerArm)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("My Lower Right Arm!")));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageCue, RightLowerArm->GetSocketLocation("None"), 3.f, 1.f, 0.0f, nullptr, nullptr);
			PlayAnimMontage(HitAnim);
			CurrentHealth = CurrentHealth - Damage;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHealth));
		}
		else if (HitResult.GetComponent() == LeftCalfLeg)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("My Left calf!")));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageCue, LeftCalfLeg->GetSocketLocation("None"), 3.f, 1.f, 0.0f, nullptr, nullptr);
			PlayAnimMontage(HitAnim);
			CurrentHealth = CurrentHealth - Damage;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHealth));
		}
		else if (HitResult.GetComponent() == RightCalfLeg)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("My Right calf!")));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageCue, RightCalfLeg->GetSocketLocation("None"), 3.f, 1.f, 0.0f, nullptr, nullptr);
			PlayAnimMontage(HitAnim);
			CurrentHealth = CurrentHealth - Damage;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHealth));
		}
		else if (HitResult.GetComponent() == LeftThighLeg)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("My left thigh!")));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageCue, LeftThighLeg->GetSocketLocation("None"), 3.f, 1.f, 0.0f, nullptr, nullptr);
			PlayAnimMontage(HitAnim);
			CurrentHealth = CurrentHealth - Damage;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHealth));
		}
		else if (HitResult.GetComponent() == RightThighLeg)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("My Right thigh!")));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageCue, RightThighLeg->GetSocketLocation("None"), 3.f, 1.f, 0.0f, nullptr, nullptr);
			PlayAnimMontage(HitAnim);
			CurrentHealth = CurrentHealth - Damage;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHealth));
		}
		else if (HitResult.GetComponent() == RightUpperArm)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("My right upper arm!")));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageCue, RightUpperArm->GetSocketLocation("None"), 3.f, 1.f, 0.0f, nullptr, nullptr);
			PlayAnimMontage(HitAnim);
			CurrentHealth = CurrentHealth - Damage;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHealth));
		}
		else if (HitResult.GetComponent() == LeftUpperArm)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("My left upper arm!")));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageCue, LeftUpperArm->GetSocketLocation("None"), 3.f, 1.f, 0.0f, nullptr, nullptr);
			PlayAnimMontage(HitAnim);
			CurrentHealth = CurrentHealth - Damage;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHealth));
		}

		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NoWork")));
		}
	}
	return 0.0f;
}

/*rifle code*/
void AMainCharacter::RifleEquip()
{
	if (bHoldingGun)
	{
		RifleUnEquip();
	}
	else
	{
		SwordUnequip();
		PlayAnimMontage(EquipGun);
		GetWorld()->GetTimerManager().SetTimer(EquipGunTimer, this, &AMainCharacter::AttachGunToHand, 0.9f, false);
	}
}

void AMainCharacter::AttachGunToHand()
{
	SM_Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Gun_Hand"));
	bHoldingGun = true;
	WeaponType = EWeaponEnum::Gun;
}

void AMainCharacter::RifleUnEquip()
{
	SM_Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RifleOnBack"));
	bHoldingGun = false;
	WeaponType = EWeaponEnum::NoWeapon;
}

void AMainCharacter::RifleFire()
{
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = Muzzle->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector EndTrace = ((ForwardVector * 5000) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	if (bHoldingGun)
	{
		if (CurrentAmmo > 0)
		{
			CurrentAmmo = CurrentAmmo - 1;
			UGameplayStatics::PlaySoundAtLocation(GetWorld(),RifleFireCue,Muzzle->GetSocketLocation("None"),2.f,1.f,0.0f,nullptr,nullptr);
			//PlayAnimMontage(RifleFireAnim);
			if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
			{
				UGameplayStatics::ApplyPointDamage(HitResult->GetActor(), 20.f, HitResult->ImpactPoint, *HitResult, NULL, this, NULL);
				DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, 5.f);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *HitResult->Actor->GetName()));
			}
		}
	}
}

void AMainCharacter::RifleReload()
{
	if (bHoldingGun)
	{
		if (bIsADS)
		{
			PlayAnimMontage(ReloadIronSightsAnim);
		}
		else
		{
			PlayAnimMontage(ReloadAnim);
		}
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AMainCharacter::AmmoInGun, 2.f, false);
	}
}

void AMainCharacter::AmmoInGun()
{
	CurrentAmmo = MaxAmmo;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Reload")));
}

void AMainCharacter::Aim()
{
	if (bHoldingGun)
	{
		bIsADS = true;
		CameraBoom->TargetArmLength = 200.f;
		CameraBoom->SocketOffset = FVector(50.0f, 50.0f, 50.f);
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void AMainCharacter::StopAiming()
{
	if (bHoldingGun)
	{
		bIsADS = false;
		CameraBoom->TargetArmLength = 750.0f;
		CameraBoom->SocketOffset = FVector(0.f, 0.f, 100.0f);
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

/*Sword Code*/
void AMainCharacter::SwordEquip()
{
	if (bHoldingSword)
	{
		SwordUnequip();
	}
	else
	{
		RifleUnEquip();
		PlayAnimMontage(EquipSword);
		GetWorld()->GetTimerManager().SetTimer(EquipSwordTimer, this, &AMainCharacter::AttachSwordToHand, 0.6f, false);
	}
}

void AMainCharacter::AttachSwordToHand()
{
	SM_Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Sword_Hand"));
	bHoldingSword = true;
	WeaponType = EWeaponEnum::Sword;
}

void AMainCharacter::SwordUnequip()
{
	SM_Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("SwordInSheath"));
	bHoldingSword = false;
	WeaponType = EWeaponEnum::NoWeapon;
}

void AMainCharacter::UnEquipAll()
{
	SwordUnequip();
	RifleUnEquip();
}

/*Attack Code*/
void AMainCharacter::Attack()
{
	if (bHoldingGun)
	{
		RifleFire();
	}
	else if (bHoldingSword)
	{
		//sword logic here
		//Heavy sword swing
		if (!Guarding)
		{
			if (!bIsSwinging)
			{
				switch (HeavySwordAttackNumber)
				{
				default:
					break;
				case 0:
					PlayAnimMontage(HeavyAttack01);
					bIsSwinging = true;
					HeavySwordAttackNumber = 1;
					bCanSwing = false;
					GetWorld()->GetTimerManager().SetTimer(SwordTimer, this, &AMainCharacter::TimerReset, 1.f, false);
					break;
				case 1:
					PlayAnimMontage(HeavyAttack02);
					bIsSwinging = true;
					HeavySwordAttackNumber = 0;
					bCanSwing = false;
					GetWorld()->GetTimerManager().SetTimer(SwordTimer, this, &AMainCharacter::TimerReset, 1.f, false);
					break;
				}
			}
		}
	}
}

void AMainCharacter::LightAttack()
{
	if (bHoldingSword)
	{
		//sword logic here
		//bCanSwing = true;
		if (!Guarding)
		{

			if (!bIsSwinging)
			{
				switch (SwordAttackNumber)
				{
				default:
					break;
				case 0:
					PlayAnimMontage(LightAttack01);
					bIsSwinging = true;
					SwordAttackNumber = 1;
					bCanSwing = false;
					GetWorld()->GetTimerManager().SetTimer(SwordTimer, this, &AMainCharacter::TimerReset, 1.f, false);
					break;
				case 1:
					PlayAnimMontage(LightAttack02);
					bIsSwinging = true;
					SwordAttackNumber = 0;
					bCanSwing = false;
					GetWorld()->GetTimerManager().SetTimer(SwordTimer, this, &AMainCharacter::TimerReset, 1.f, false);
					break;
				}
			}
		}
	}
}

void AMainCharacter::Roll()
{
	if (bIsSwinging)
	{}
	else
	{
		PlayAnimMontage(RollAnim);
		bIsRolling = true;
		GetWorld()->GetTimerManager().SetTimer(RollTimer, this, &AMainCharacter::TimerReset, 0.8f, false);
	}
}

void AMainCharacter::TimerReset()
{
	bIsSwinging = false;
	bIsRolling = false;
}



/*Lock on Code*/
void AMainCharacter::LockOn()
{
	TArray<AActor*>FoundActors;
	//find enemy character
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainCharacter::StaticClass(), FoundActors);

	for (int32 i = 0; i < FoundActors.Num(); i++)
	{
		if (FoundActors.Num() > 0)
		{
			if (this)
			{
				EnemyFound = Cast<AMainCharacter>(FoundActors[i]);
				EnemyFound02 = Cast<AMainCharacter>(FoundActors[0]);
				if (EnemyFound != this)
				{
					bIsLockedOn = true;
				}
				else if (EnemyFound02 != this)
				{
					bIsLockedOn = true;
				}
				else
				{
					bIsLockedOn = false;
				}
			}
			/*
			if (bIsPlayerOne)
			{
				//EnemyFound = Cast<AMainCharacter>(FoundActors[FMath::RandRange(0, FoundActors.Num() - 1)]);
				EnemyFound = Cast<AMainCharacter>(FoundActors[i]);
				bIsLockedOn = true;
			}
			else
			{
				EnemyFound02 = Cast<AMainCharacter>(FoundActors[0]);
				bIsLockedOn = true;
			}
				*/
		}
	}
}

void AMainCharacter::LockOnSetter()
{
	if (bIsLockedOn)
	{
		bIsLockedOn = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		//bIsLockedOn = true;
		LockOn();
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void AMainCharacter::UpdatePlayerRot()
{
	if (EnemyFound != this)
	{
		//controller rotation, don't get yaw
		FRotator ContRot = FRotator(Controller->GetControlRotation().Pitch, 0.0f, Controller->GetControlRotation().Roll);
		//camera location
		FVector CameraLoc = FollowCamera->GetComponentLocation();
		//enemy location
		//AMainCharacter* EnemyFound = cast<AMainCharacter>(;
		FVector EnemeyTarget = EnemyFound->GetActorLocation();
		//FVector EnemeyTarget = EnemyFound->GetDefaultObject<AMainCharacter>()->GetActorLocation();
		//camera look at location
		FRotator CameraLookAt = UKismetMathLibrary::FindLookAtRotation(CameraLoc, EnemeyTarget);
		//set the smooth rotation
		FRotator LookAtUpdate = UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), CameraLookAt, DeltaSeconds, 5.0f);
		//gets all values to set the rotation of the camera
		FRotator CameraRot = FRotator(Controller->GetControlRotation().Pitch, LookAtUpdate.Yaw, Controller->GetControlRotation().Roll);
		//applies the values to the camera
		Controller->SetControlRotation(CameraRot);

		//Character rot				
		FRotator PawnRot = GetWorld()->GetFirstPlayerController()->GetControlRotation();
		//character location
		FVector PawnLoc = this->GetActorLocation();
		//has the player character look at the enemy
		FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(PawnLoc, EnemeyTarget);
		//sets the character rotation
		this->SetActorRelativeRotation(LookAt);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("GotUinMySights")));
	}
	else if (EnemyFound02 != this)
	{
		//controller rotation, don't get yaw
		FRotator ContRot = FRotator(Controller->GetControlRotation().Pitch, 0.0f, Controller->GetControlRotation().Roll);
		//camera location
		FVector CameraLoc = FollowCamera->GetComponentLocation();
		//enemy location
		//AMainCharacter* EnemyFound = cast<AMainCharacter>(;
		FVector EnemeyTarget02 = EnemyFound02->GetActorLocation();
		//FVector EnemeyTarget = EnemyFound->GetDefaultObject<AMainCharacter>()->GetActorLocation();
		//camera look at location
		FRotator CameraLookAt = UKismetMathLibrary::FindLookAtRotation(CameraLoc, EnemeyTarget02);
		//set the smooth rotation
		FRotator LookAtUpdate = UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), CameraLookAt, DeltaSeconds, 5.0f);
		//gets all values to set the rotation of the camera
		FRotator CameraRot = FRotator(Controller->GetControlRotation().Pitch, LookAtUpdate.Yaw, Controller->GetControlRotation().Roll);
		//applies the values to the camera
		Controller->SetControlRotation(CameraRot);

		//Character rot				
		FRotator PawnRot = GetWorld()->GetFirstPlayerController()->GetControlRotation();
		//character location
		FVector PawnLoc = this->GetActorLocation();
		//has the player character look at the enemy
		FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(PawnLoc, EnemeyTarget02);
		//sets the character rotation
		this->SetActorRelativeRotation(LookAt);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("GotUinMySights")));
	}
		

}

