// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "OtherPlayer.generated.h"

/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API AOtherPlayer : public AMainCharacter
{
	GENERATED_BODY()
	
public:

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LockOnSystem)
	//	AMainCharacter* EnemyFound;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerRot();
	
	
};
