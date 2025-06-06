// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InkableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInkableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INKMARK_API IInkableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Applied ink effect and damage, override and do whatever you want
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InkObjectAttack")
	void InkObject(int Damage);

	virtual void InkObject_Implementation(int Damage) = 0;
};
