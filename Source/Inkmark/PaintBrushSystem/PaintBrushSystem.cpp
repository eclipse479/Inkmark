// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintBrushSystem/PaintBrushSystem.h"

#include "Engine/DecalActor.h"


void UPaintBrushSystem::CastPaintTrace(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius)
{
	UWorld* world = GetOuter()->GetWorld();

	// Get the data paint will hit and use
	TArray<TEnumAsByte<EObjectTypeQuery>> objTypes = PaintBrushCollisionData.ObjectTypes;
	bool IgnoreSelf = PaintBrushCollisionData.bIgnoreSelf;
	TEnumAsByte<EDrawDebugTrace::Type> debugType = PaintBrushCollisionData.DrawDebugType;
	TArray<AActor*> actorsToIgnore = PaintBrushCollisionData.ActorsToIgnore;
	TArray<FHitResult>& hits = PaintBrushCollisionData.OutHits;

	// If not hitting anything
	FLinearColor traceNotHitColour = PaintBrushCollisionData.TraceColor;
	FLinearColor traceHitColour = PaintBrushCollisionData.TraceHitColor;

	float drawTime = PaintBrushCollisionData.DrawTime;

	// Has hit stuff
	bool HasHit = UKismetSystemLibrary::SphereTraceMultiForObjects(world, Start, End, Radius, objTypes, true, actorsToIgnore, debugType, hits, IgnoreSelf, traceNotHitColour, traceHitColour, drawTime);
}

void UPaintBrushSystem::SpawnPaintDecal(UWorld& world, FVector location, FVector Normal)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = nullptr;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// spawnParams.

	

	ADecalActor* decal = world.SpawnActor<ADecalActor>(DecalObject, location, Normal.Rotation());
}
