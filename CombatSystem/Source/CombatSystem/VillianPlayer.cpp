// Fill out your copyright notice in the Description page of Project Settings.

#include "VillianPlayer.h"


// Sets default values
AVillianPlayer::AVillianPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVillianPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVillianPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVillianPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

