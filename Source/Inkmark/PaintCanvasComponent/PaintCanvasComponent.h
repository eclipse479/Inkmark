// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Components/DecalComponent.h"

#include "PaintBrushSystem/PaintBrushSystem.h"

#include "PaintCanvasComponent.generated.h"

class UCanvas;
class UTexture2D;

USTRUCT(BlueprintType)
struct FPaintCanvasData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Canvas Material")
	UMaterial* ParentMaterialCanvas;

	UPROPERTY(EditAnywhere, Category = "Canvas Material")
	FName PaintCanvasRenderTargetName = "RenderTarget";

	UMaterialInstanceDynamic* CanvasMaterial;
};


USTRUCT(BlueprintType)
struct FPaintBrushData
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

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class INKMARK_API UPaintCanvasComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UPaintCanvasComponent();

	virtual void BeginPlay() override;

	// Function to draw on mesh
	void DrawBrush(/*UTexture2D& brushTexture, float brushSize, */FVector2D draw_location);

	UFUNCTION()
	void GetBrushScreenPosition(FVector StartLocation, FVector EndLocation);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void DrawOntoCanvas(UTexture2D* tex2d, float brush_size, FVector2D draw_location);

	// Canvas Data
	UPROPERTY(EditAnywhere, Category = "Canvas Material")
	FPaintCanvasData PaintTargetData;

	// Brush Data
	UPROPERTY(EditAnywhere, Category = "Canvas Material")
	FPaintBrushData PaintBrushData;

	// Variables to work with
	UCanvasRenderTarget2D* RenderTarget;

	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, Category = "Targets")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTargets;

	UPROPERTY(EditAnywhere, Category = "Targets")
	TArray<TEnumAsByte<EObjectTypeQuery>> CanvasTarget;

	UPROPERTY(EditAnywhere, Category = "Decal Paint")
	TSubclassOf<ADecalActor> DecalObject;
private:
	UCanvas* CanvasToRender;

	UPaintBrushSystem* BrushSystem;
};
