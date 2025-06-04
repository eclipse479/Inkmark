// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PaintBrushSystem/PaintBrushSystem.h"

#include "enemy.h"

#include "PaintCanvasActor.generated.h"

class UPaintBrushSystem;
class UCanvas;
class ADecalActor;

class UCanvasRenderTarget2D;
class UStaticMeshComponent;

class UTexture2D;

USTRUCT(BlueprintType)
struct FPaintCanvasDataForActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Canvas Material")
	UMaterial* ParentMaterialCanvas;

	UPROPERTY(EditAnywhere, Category = "Canvas Material")
	FName PaintCanvasRenderTargetName = "RenderTarget";

	UMaterialInstanceDynamic* CanvasMaterial;
};


USTRUCT(BlueprintType)
struct FPaintBrushDataForActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Canvas Material")
	UMaterial* ParentMaterialBrush;

	UPROPERTY(EditAnywhere, Category = "Canvas Material")
	FName BrushRenderTargetName = "RenderTarget";

	UPROPERTY(EditAnywhere, Category = "Canvas Material")
	UTexture2D* BrushTexture;

	UPROPERTY(EditAnywhere, Category = "Canvas Material")
	float BrushSize = 20.0f;

	UMaterialInstanceDynamic* BrushMaterial;
};

// PaintCanvasActor Jsut allows render targets to paint onto it
UCLASS(Blueprintable, BlueprintType)
class INKMARK_API APaintCanvasActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaintCanvasActor();

	UFUNCTION(BlueprintImplementableEvent)
	void DrawOntoCanvas(UTexture2D* tex2d, float brush_size, FVector2D draw_location);

	UFUNCTION(BlueprintImplementableEvent)
	void ClearPaintCanvas();

	void AddHitToBuffer(FHitResult& hit, FVector Direction)
	{
		Hits.Add(hit);
		StartPoints.Add(hit.ImpactPoint);

		AimDirection = Direction;
	}

	UFUNCTION()
	void FireAndClearHitScans()
	{
		bool HasHitsInBuffer = Hits.Num() > 0;

		if (!HasHitsInBuffer)
			return;

		for (FVector StartPosition : StartPoints)
		{
			FHitResult hit;

			Aenemy* enemy;
			if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartPosition, (AimDirection * HitScanDistance) + StartPosition, ObjectTargets, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, hit, false))
			{
				enemy = Cast<Aenemy>(hit.GetActor());
				bool HasHitEnemy = enemy != nullptr;

				if (HasHitEnemy)
				{
					enemy->DamageDong(1);
				}
			}
		}

		// Clear for next usage
		StartPoints.Empty();
		Hits.Empty();
	}

public:
	// Start points
	TArray<FVector> StartPoints;

	// Hits results
	TArray<FHitResult> Hits;

	// How far the scan checks
	UPROPERTY(EditAnywhere, Category = TraceScan, meta = (AllowPrivateAccess = true))
	float HitScanDistance = 1000.0f;

	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, Category = "Targets")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTargets;

	FVector AimDirection;
};
