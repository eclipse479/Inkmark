// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "enemy.generated.h"

UCLASS()
class INKMARK_API Aenemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Aenemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "health")
	int MaxHealth  = 100;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "health")
	int CurrentHealth;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "ink")
	int MaxInkLevel = 100;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "ink")
	int CurrentInkLevel;

	UFUNCTION(BlueprintCallable, Category = "damage")
	void TakeDamage(int damage);
	UFUNCTION(BlueprintCallable, Category = "damage")
	void Death();
	UFUNCTION(BlueprintCallable, Category = "damage")
	void GetInked(int ink);
	UFUNCTION(BlueprintCallable, Category = "damage")
	void InkFull();
};
