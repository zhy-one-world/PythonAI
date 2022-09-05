// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameUserSettings.h"
#include "PyCharacter.h"
#include "PythonAICharacter.generated.h"


USTRUCT(Blueprintable, BlueprintType)
struct FMyStruct
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadOnly)
		float HP = 10;
	UPROPERTY(BlueprintReadOnly)
		float Armor = 100;
};

UCLASS(config=Game)
class APythonAICharacter : public APyCharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Capture, meta = (AllowPrivateAccess = "true"))
		class USceneCaptureComponent2D* CaptureComponent2D;
public:

	// Called every frame
	
	APythonAICharacter();

	virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable)
		float GetActorSpeed();

	UFUNCTION(BlueprintCallable)
		FVector GetCameraForwardVector();
//	int* GetColorDate(const TArray<FColor>& ColorArr);

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	UFUNCTION(BlueprintCallable, Category = Character)
	void MoveForward(float Value);

	/** Called for side to side input */
	UFUNCTION(BlueprintCallable, Category = Character)
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable)
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable)
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

//	int FColorDateArr[2073600 * 4];

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintCallable)
		TArray<FColor> PictureSampling(const FVector2D& RangeSize);
	UPROPERTY(BlueprintReadOnly)
	TArray<FColor> ColorDateArr;
	UFUNCTION(BlueprintCallable)
		TArray<FColor> GetColorDateArr(const TArray<FColor>& ColorArr);
//	UGameUserSettings Settings;

	UGameUserSettings* UserSetting = nullptr;

	UWorld* MyWorld = nullptr;
	float Time = 0;
	FIntPoint Resolution;

	UPROPERTY(BlueprintReadOnly)
		FMyStruct My = { 11.0f,12.0f };

//	static int* image;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

