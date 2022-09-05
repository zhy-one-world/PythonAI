// Copyright Epic Games, Inc. All Rights Reserved.

#include "PythonAICharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Modules/ModuleManager.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Misc/FileHelper.h"
#include "ImageUtils.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// APythonAICharacter

//int* APythonAICharacter::image = NULL;
APythonAICharacter::APythonAICharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	PrimaryActorTick.bCanEverTick = true;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	if (FollowCamera && CaptureComponent2D)
	{
		CaptureComponent2D->SetupAttachment(FollowCamera);
	}
	

	if (GEngine)
	{
		UserSetting = GEngine->GetGameUserSettings();
		if (UserSetting)
		{
			Resolution = UserSetting->GetScreenResolution();
		}
	}
	
//	image = new int[4*Resolution.X*Resolution.Y];
//	int FColorDateArr[4 * (Resolution.X) * (Resolution.Y)];
//	4 * (Resolution.X) * (Resolution.Y) Resolution为分辨率信息


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void APythonAICharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APythonAICharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APythonAICharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APythonAICharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APythonAICharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APythonAICharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APythonAICharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &APythonAICharacter::OnResetVR);
}

void APythonAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Time += DeltaTime;
	if (Time > DeltaTime)
	{
		PictureSampling(FVector2D(800, 600));
//		GetColorDate(ColorDateArr);

		//调用蓝图python通知截取成功了
		Time = 0;
	}
}

float APythonAICharacter::GetActorSpeed()
{
	if (this)
	{
		FVector Velocity = this->GetVelocity();
		float Speed = FMath::Sqrt(FMath::Square<float>((float)Velocity.X) + FMath::Square<float>((float)Velocity.Y) + FMath::Square<float>((float)Velocity.Z));
		return Speed;
	}
	else
	{
		return 0.0f;
	}

}

FVector APythonAICharacter::GetCameraForwardVector()
{
	if (!CaptureComponent2D)
	{
		return FVector(0, 0, 0);
	}
	FVector CameraForwardVector = CaptureComponent2D->GetForwardVector();
	return CameraForwardVector;
}

/*int* APythonAICharacter::GetColorDate(const TArray<FColor>& ColorArr)
{
	for (int32 i = 0; i < ColorArr.Num(); i++)
	{
		for (int32 j = 4 * i; j < 4 * (i + 1); j++)
		{
			image[j] = ColorArr[i].B;
			image[j+1] = ColorArr[i].G;
			image[j+2] = ColorArr[i].R;
			image[j+3] = ColorArr[i].A;
		}
	}
//	UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("aa") );
	return image;
}
*/


TArray<FColor> APythonAICharacter::PictureSampling(const FVector2D& RangeSize)
{
	if (CaptureComponent2D && CaptureComponent2D->TextureTarget)
	{
		CaptureComponent2D->TextureTarget->InitAutoFormat(RangeSize.X, RangeSize.Y);
		CaptureComponent2D->UpdateContent();
		auto Lab = [=]()
		{
			FTextureRenderTargetResource* RenderResource = CaptureComponent2D->TextureTarget->GameThread_GetRenderTargetResource();
			if (RenderResource)
			{
					RenderResource->ReadPixels(ColorDateArr);
//					UE_LOG(LogTemp, Warning, TEXT("%s"), *(ColorDateArr[1].ToString()));
					return ColorDateArr;
			}
			ColorDateArr.Empty();
			return ColorDateArr;
		};
		FTimerHandle TimerHandle;
		MyWorld = GetWorld();
		if (MyWorld)
		{
			MyWorld->GetTimerManager().SetTimer(TimerHandle, Lab, 0.0001f, false, 0);
		}
		return ColorDateArr;
	}
	ColorDateArr.Empty();
	return ColorDateArr;
}


TArray<FColor> APythonAICharacter::GetColorDateArr(const TArray<FColor>& ColorArr)
{
	return ColorArr;
}

void APythonAICharacter::OnResetVR()
{
	// If PythonAI is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in PythonAI.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APythonAICharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void APythonAICharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void APythonAICharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APythonAICharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APythonAICharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APythonAICharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
