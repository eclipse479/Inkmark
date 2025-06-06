// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "Kismet/KismetMathLibrary.h"
#include "DamageInterface/InkableInterface.h"

#include "InkmarkCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UPaintCanvasComponent;
class APaintCanvasActor;

class UBoxComponent;

class UAnimMontage;

struct FInputActionValue;

USTRUCT(BlueprintType)
struct FAttackAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Attack)
	TArray<UAnimMontage*> AttackMontages;
};

// Player Stat Values for health
USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_BODY()

	void InitStats()
	{
		CurrentHealth = MaxHealth;
		CurrentInk = MaxInk;
		CurrentAttack = BaseAttack;
		CurrentPaintGauge = PaintGauge;
	}

	// Health Actor or entity can have
	UPROPERTY(EditAnywhere, Category = Stats)
	int MaxHealth = 100;

	// Max ink actor or entity
	UPROPERTY(EditAnywhere, Category = Stats)
	int MaxInk = 20;

	// Base Attack Value for actor or entity
	UPROPERTY(EditAnywhere, Category = Stats)
	float BaseAttack = 2;

	// Max Attack Value for actor or entity when in overflow
	UPROPERTY(EditAnywhere, Category = Stats)
	int OverflowAttack = 100;

	// Base Attack Value for actor or entity
	UPROPERTY(EditAnywhere, Category = Stats)
	int PaintGauge = 10;

	//***************************************//
	// Mutable variables

	int CurrentHealth = 0;

	int CurrentInk = 0;

	float CurrentAttack = 2;

	int CurrentPaintGauge = 0;

	bool bOverflow = false;

	bool bPaintBrushInputHeld = false;
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AInkmarkCharacter : public ACharacter, public IInkableInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Paint Button Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PaintAction;

	/** Paint Button Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/****************************************** Paint Canvas Start ***********************************************/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	APaintCanvasActor* PaintCanvasActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UPaintCanvasComponent* PaintCanvasComp;

	/****************************************** Paint Canvas End ************************************************/

	//******************************************************************************************************//
	//*********************************** Player Stats and Abilities **************************************//
	//******************************************************************************************************//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PaintCanvas, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APaintCanvasActor> PaintCanvasType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PaintCanvas, meta = (AllowPrivateAccess = "true"))
	FAttackAnimation AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	FPlayerStats PlayerStatValues;

	//******************************************************************************************************//
	//*********************************** Player Stats and Abilities **************************************//
	//******************************************************************************************************//
public:
	AInkmarkCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	/** Called for Tracking Mouse Position **/
	void TrackMousePosition(const FInputActionValue& Value);

	/** Called for Tracking Mouse Position **/
	void CancelPaint(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	// Play Montage of attack animation, why like this?
	void PlayAnimation()
	{
		int32 AnimIndex = UKismetMathLibrary::RandomIntegerInRange(0, AttackAnimation.AttackMontages.Num() - 1);

		UAnimMontage* anim = AttackAnimation.AttackMontages[AnimIndex];

		bool HasAttackAnim = anim != nullptr;

		if (HasAttackAnim)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(anim, 1.0f);
		}
	}


	UFUNCTION(BlueprintCallable, Category = PaintCanvas)
	void EnableHitBox();

	UFUNCTION(BlueprintCallable, Category = PaintCanvas)
	void DisableHitBox();

	// Hit damage code
	UFUNCTION(BlueprintCallable, Category = PaintCanvas)
	void HitBoxDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Increase Ink Amount
	UFUNCTION(BlueprintCallable, Category = PaintCanvas)
	void IncreaseInkAmount(int value);

	// Decrease Ink Amount
	UFUNCTION(BlueprintCallable, Category = PaintCanvas)
	void DecreaseInkAmount(int value);

	// Adjust damage based on amount of ink
	UFUNCTION(BlueprintCallable, Category = PaintDamage)
	void ScaleDamage();

	// Plays when damage is taken
	virtual void InkObject_Implementation(float Damage) override;

public:
	/** Melee Events **/
	UFUNCTION(BlueprintImplementableEvent)
	void StartMeleeHitEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void EndMeleeHitEvent();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Hit Box 
	UBoxComponent* MeleeHitBox;
};

