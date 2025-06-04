// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Engine/DecalActor.h"

#include "PaintBrushSystem.generated.h"


class UNiagaraSystem;

// Paint Collision Data such Targets and ignore
USTRUCT(BlueprintType)
struct FPaintCollisionData
{
	GENERATED_BODY()

	FPaintCollisionData() {}

	// Object types to hit
	UPROPERTY(EditAnywhere, Category = "Paint Targets")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	// Actors to ignore
	UPROPERTY(EditAnywhere, Category = "Paint Targets")
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, Category = "Paint Targets")
	TArray<FHitResult> OutHits;

	////// Debug type
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bIgnoreSelf = true;

	UPROPERTY(EditAnywhere, Category = "Debug")
	FLinearColor TraceColor;

	UPROPERTY(EditAnywhere, Category = "Debug")
	FLinearColor TraceHitColor;

	UPROPERTY(EditAnywhere, Category = "Debug")
	float DrawTime = 1.0f;
};

/** PaintBrushSystem
 * 
 * Just a library to allow anything to cast a trace from anywhere in the world and hit stuff
 */
UCLASS()
class INKMARK_API UPaintBrushSystem : public UObject
{
	GENERATED_BODY()
	
public:
	// Literally just a sphere trace that just splashes decals onto meshes into the world
	void CastPaintTrace(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius);

	// Spawn decal at location and facing direction
	void SpawnPaintDecal(FVector location, FVector Normal);

public:
	UPROPERTY(BlueprintReadWrite, Category = "Paint Brush Hit Data")
	FPaintCollisionData PaintBrushCollisionData;

	UPROPERTY(BlueprintReadWrite, Category = "Paint Brush Hit Data")
	TSubclassOf<ADecalActor> DecalObject;
};

