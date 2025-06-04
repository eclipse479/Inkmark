// Fill out your copyright notice in the Description page of Project Settings.


#include "enemy.h"

// Sets default values
Aenemy::Aenemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aenemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

// Called every frame
void Aenemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Aenemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void Aenemy::Death()
{
	//play death animation
	AActor::Destroy();
}

void Aenemy::GetInked(int ink)
{
	CurrentInkLevel += ink;
	if (CurrentInkLevel > MaxInkLevel)
		InkFull();
}

void Aenemy::InkFull()
{
}

void Aenemy::TakeDamage(int damage)
{
	CurrentHealth -= damage;
	if (CurrentHealth <= 0)
		Death();
}
