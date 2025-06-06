// Copyright Epic Games, Inc. All Rights Reserved.

#include "InkmarkCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Kismet/GameplayStatics.h"

#include "enemy.h"

#include "PaintCanvasComponent/PaintCanvasComponent.h"
#include "PaintBrushSystem/PaintCanvasActor.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AInkmarkCharacter

AInkmarkCharacter::AInkmarkCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller


	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	PaintCanvasComp = CreateDefaultSubobject<UPaintCanvasComponent>(TEXT("PaintCanvasComp"));
	PaintCanvasComp->SetupAttachment(CameraBoom);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AInkmarkCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Create a canvas component
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	PaintCanvasActor = GetWorld()->SpawnActor<APaintCanvasActor>(PaintCanvasType, spawnParams);

	USceneComponent& CameraChildComp = *FollowCamera->GetChildComponent(0);

	FAttachmentTransformRules attachmentRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	//attachmentRules.LocationRule = EAttachmentRule::SnapToTarget;
	//attachmentRules.RotationRule = EAttachmentRule::KeepRelative;
	//attachmentRules.ScaleRule = EAttachmentRule::KeepRelative;

	PaintCanvasActor->AttachToComponent(FollowCamera, attachmentRules);
	PaintCanvasActor->SetActorRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	PaintCanvasActor->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	// Get MeleeHitBox
	MeleeHitBox = GetComponentByClass<UBoxComponent>();
	MeleeHitBox->OnComponentBeginOverlap.AddDynamic(this, &AInkmarkCharacter::HitBoxDamage);

	// Init Player Stats
	PlayerStatValues.InitStats();
}

void AInkmarkCharacter::EnableHitBox()
{
	MeleeHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AInkmarkCharacter::DisableHitBox()
{
	MeleeHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AInkmarkCharacter::HitBoxDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bool HasHit = OtherActor != nullptr && OtherActor != this;

	if (HasHit)
	{
		Aenemy* enemy = Cast<Aenemy>(OtherActor);
		// IInkableInterface* InkHitable = 

		// Has overlapped Inkable
		bool HitInkable = OtherActor->GetClass()->ImplementsInterface(UInkableInterface::StaticClass());

		bool HitEnemy = enemy != nullptr;

		// Hit an enemy
		if (HitEnemy)
		{
			//enemy->DamageDong(PlayerStatValues.CurrentAttack);
		}

		// Hit an Inkable object

		if (HitInkable)
		{
			IInkableInterface::Execute_InkObject(OtherActor, PlayerStatValues.CurrentAttack);
			//InkHitable->InkObject(PlayerStatValues.CurrentAttack);
		}
	}
}

void AInkmarkCharacter::IncreaseInkAmount(int value)
{
	PlayerStatValues.CurrentInk += value;

	bool GreaterThanMaxInk = PlayerStatValues.CurrentInk > PlayerStatValues.MaxInk;

	PlayerStatValues.bOverflow = GreaterThanMaxInk;
}

void AInkmarkCharacter::DecreaseInkAmount(int value)
{
	PlayerStatValues.CurrentInk -= value;

	bool GreaterThanMaxInk = PlayerStatValues.CurrentInk > PlayerStatValues.MaxInk;

	PlayerStatValues.bOverflow = GreaterThanMaxInk;
}

void AInkmarkCharacter::ScaleDamage()
{
	// If in overflow, then just 
	if (PlayerStatValues.bOverflow)
	{
		PlayerStatValues.CurrentAttack = PlayerStatValues.OverflowAttack;
		return;
	}

	// Otherwise set damage value to this
	int newDamage = PlayerStatValues.BaseAttack + (PlayerStatValues.CurrentInk / PlayerStatValues.BaseAttack);
	PlayerStatValues.CurrentAttack = newDamage;
}

void AInkmarkCharacter::InkObject_Implementation(float Damage)
{
	int& Health = PlayerStatValues.CurrentHealth;
	bool HasHealth = Health > 0;

	// If has health greater than zero
	if (HasHealth)
	{
		Health -= Damage;
	}
	else
	{
		// Die
	}

	UE_LOG(LogTemp, Warning, TEXT("Damage Player"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AInkmarkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInkmarkCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInkmarkCharacter::Look);

		// Paint
		EnhancedInputComponent->BindAction(PaintAction, ETriggerEvent::Triggered, this, &AInkmarkCharacter::TrackMousePosition);
		EnhancedInputComponent->BindAction(PaintAction, ETriggerEvent::None, this, &AInkmarkCharacter::CancelPaint);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AInkmarkCharacter::PlayAnimation);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AInkmarkCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AInkmarkCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AInkmarkCharacter::TrackMousePosition(const FInputActionValue& Value)
{
	bool bCanUse = PlayerStatValues.CurrentInk > 0;

	if (!bCanUse)
		return;

	float value = Value.Get<float>();

	bool IsUsed = Controller && value >= 1.0f;

	APlayerController& PC = *Cast<APlayerController>(Controller);

	PC.SetIgnoreLookInput(true);

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0);

	PC.SetShowMouseCursor(true);

	float MouseX, MouseY;
	FVector2D MousePosition;
	FVector WorldLoc = FollowCamera->GetComponentLocation();
	FVector WorldDir = FollowCamera->GetForwardVector();

	// Tells input if this was used
	PlayerStatValues.bPaintBrushInputHeld = IsUsed;

	PC.GetMousePosition(MouseX, MouseY);
	MousePosition = FVector2D(MouseX, MouseY);

	PC.DeprojectMousePositionToWorld(WorldLoc, WorldDir);
	//UGameplayStatics::DeprojectScreenToWorld(&PC, MousePosition, WorldLoc, WorldDir);

	PaintCanvasActor->SetActorHiddenInGame(false);

	PaintCanvasComp->GetBrushScreenPosition(WorldLoc, WorldLoc + (WorldDir * 1000.0f));
}

void AInkmarkCharacter::CancelPaint(const FInputActionValue& Value)
{
	float value = Value.Get<float>();

	// bool IsUsed = Controller && value >= 1.0f;

	APlayerController& PC = *Cast<APlayerController>(Controller);

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

	PC.SetInputMode(FInputModeGameOnly());

	PC.SetShowMouseCursor(false);
	PC.ResetIgnoreLookInput();

	// Only decrease amount when input was held
	if (PlayerStatValues.bPaintBrushInputHeld)
	{
		DecreaseInkAmount(1);
		PlayerStatValues.bPaintBrushInputHeld = false;
	}

	// This wil trigger every time
	PaintCanvasActor->FireAndClearHitScans();

	PaintCanvasActor->ClearPaintCanvas();
	PaintCanvasActor->SetActorHiddenInGame(true);
}
