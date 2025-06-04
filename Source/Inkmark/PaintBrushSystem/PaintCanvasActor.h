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
	APaintCanvasActor()
	{
		BrushSystem = NewObject<UPaintBrushSystem>();
	}

	UFUNCTION(BlueprintImplementableEvent)
	void DrawOntoCanvas(UTexture2D* tex2d, float brush_size, FVector2D draw_location);

	UFUNCTION(BlueprintImplementableEvent)
	void ClearPaintCanvas();

	void AddHitToBuffer(FVector Start, FVector Direction)
	{
		StartPoints.Add(Start);
		EndPoints.Add(Direction);
		//AimDirection = Direction;
	}

	UFUNCTION()
	void FireAndClearHitScans()
	{
		bool HasHitsInBuffer = StartPoints.Num() > 0;

		if (!HasHitsInBuffer)
			return;

		for (int32 index = 0; index < StartPoints.Num(); ++index)
		{
			FHitResult hit;
			Aenemy* enemy;
			FVector StartPosition = StartPoints[index];
			FVector EndPosition = EndPoints[index];


			if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartPosition, EndPosition, ObjectTargets, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, hit, false))
			{
				enemy = Cast<Aenemy>(hit.GetActor());
				bool HasHitEnemy = enemy != nullptr;

				if (HasHitEnemy)
				{
					enemy->DamageDong(1);
				}
			}
			else
			{
				BrushSystem->DecalObject = DecalObject;
				BrushSystem->SpawnPaintDecal(*GetWorld(), hit.ImpactPoint, hit.ImpactNormal);
			}
		}

		// Clear for next usage
		StartPoints.Empty();
		EndPoints.Empty();
		//Hits.Empty();
	}

public:
	// Start points
	TArray<FVector> StartPoints;
	// End Points
	TArray<FVector> EndPoints;

	// Hits results
	TArray<FHitResult> Hits;

	// How far the scan checks
	UPROPERTY(EditAnywhere, Category = TraceScan, meta = (AllowPrivateAccess = true))
	float HitScanDistance = 1000.0f;

	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, Category = "Decal Paint")
	TSubclassOf<ADecalActor> DecalObject;

	UPROPERTY(EditAnywhere, Category = "Targets")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTargets;

	FVector AimDirection;

	UPaintBrushSystem* BrushSystem;
};
