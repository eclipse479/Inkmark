// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintCanvasComponent/PaintCanvasComponent.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetMathLibrary.h"

#include "PaintBrushSystem/PaintCanvasActor.h"
#include "enemy.h"

UPaintCanvasComponent::UPaintCanvasComponent()
{
	
}

void UPaintCanvasComponent::BeginPlay()
{
	Super::BeginPlay();

	//BrushSystem = NewObject<UPaintBrushSystem>();
	//BrushSystem->DecalObject = DecalObject;

	//// Create target to paint on
	//RenderTarget = NewObject<UCanvasRenderTarget2D>();
	//
	//RenderTarget->SizeX = 1024;
	//RenderTarget->SizeY = 1024;
	//RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
	//RenderTarget->ClearColor = FLinearColor::White;

	//UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), RenderTarget, RenderTarget->ClearColor);

	//// Dynamic Material to apply canvas
	//PaintTargetData.CanvasMaterial = CreateDynamicMaterialInstance(0, PaintTargetData.ParentMaterialCanvas, "None");
	//PaintTargetData.CanvasMaterial->SetTextureParameterValue(PaintTargetData.PaintCanvasRenderTargetName, RenderTarget);

	//// Set material to what is made above
	//SetMaterial(0, PaintTargetData.CanvasMaterial);

	//// Create the brush
	//PaintBrushData.BrushMaterial = CreateDynamicMaterialInstance(0, PaintBrushData.ParentMaterialBrush, "None");
}

void UPaintCanvasComponent::DrawBrush(/*UTexture2D& brushTexture, float brushSize, */FVector2D draw_location)
{
	FDrawToRenderTargetContext Context;

	// Context.RenderTarget = RenderTarget;
	

	float BrushSize = PaintBrushData.BrushSize;
	FVector2D CanvasDrawLocation = draw_location /*- (BrushSize * 0.5f)*/;
	FVector2D CanvasScreen = CanvasDrawLocation - (BrushSize * 0.5f);
	CanvasToRender = NewObject<UCanvas>();

	UTexture& brushMat = *PaintBrushData.BrushTexture;

	//CanvasToRender->K2_DrawPolygon(PaintBrushData.BrushTexture, CanvasDrawLocation, CanvasScreen, 16, FLinearColor::White);
	PaintBrushData.BrushMaterial->SetTextureParameterValue(PaintBrushData.BrushRenderTargetName, &brushMat);

	
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), RenderTarget, CanvasToRender, CanvasScreen, Context);
	CanvasToRender->K2_DrawMaterial(PaintBrushData.BrushMaterial, CanvasDrawLocation, FVector2D(BrushSize, BrushSize), FVector2D(0.5f, 0.5f));
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
}

void UPaintCanvasComponent::GetBrushScreenPosition(FVector StartLocation, FVector EndLocation)
{
	// UKismetSystemLibrary::PrintString(GetWorld(), screenPos.ToString(), true, true);

	//TArray<FHitResult> hits;

	//if (UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, ObjectTargets, true, ActorsToIgnore, EDrawDebugTrace::None, hits, true))
	//{
	//	// check for hits
	//	for (FHitResult& Hit : hits)
	//	{
	//		Aenemy* enemy = Cast<Aenemy>(Hit.GetActor());

	//		if (enemy)
	//		{
	//			// BrushSystem->SpawnPaintDecal(Hit.ImpactPoint, Hit.ImpactNormal);
	//			//enemy->TakeDamage(2);
	//			//enemy->GetInked(1);
	//			enemy->DamageDong(1);
	//		}
	//	}
	//}

	FHitResult hit;


	//UPaintCanvasComponent* PaintComp;
	APaintCanvasActor* paintActor;
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartLocation, EndLocation, CanvasTarget, true, ActorsToIgnore, EDrawDebugTrace::None, hit, false))
	{
		paintActor = Cast<APaintCanvasActor>(hit.GetActor());

		// Has comp
		bool IsPaintCanvas = paintActor != nullptr;

		if (IsPaintCanvas)
		{
			FVector2D location;

			UGameplayStatics::FindCollisionUV(hit, 0, location);

			paintActor->DrawOntoCanvas(PaintBrushData.BrushTexture, PaintBrushData.BrushSize, location);
			paintActor->AddHitToBuffer(StartLocation, EndLocation);
		}
	}

	// Paint to the screen
	/*DrawBrush(screenPos);*/
}
