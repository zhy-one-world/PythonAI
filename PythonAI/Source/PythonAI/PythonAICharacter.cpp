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
#include "TextReadWrite.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// APythonAICharacter

//int* APythonAICharacter::image = NULL;
APythonAICharacter::APythonAICharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	PrimaryActorTick.bCanEverTick = true;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//if (FollowCamera)
	//{
	//	FollowCamera->SetupAttachment(RootComponent);
	//	FollowCamera->SetRelativeLocation(FVector(-250, 0, 0));
	//}

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
	
	if (this)
	{
		this->PythonModule = "PCharacter";
		this->PythonClass = "Character";
	}
}


void APythonAICharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APythonAICharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APythonAICharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APythonAICharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APythonAICharacter::LookUpAtRate);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &APythonAICharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APythonAICharacter::TouchStopped);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &APythonAICharacter::OnResetVR);
}

void APythonAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Time += DeltaTime;
	if (Time > DeltaTime)
	{
		PictureSampling(FVector2D(800, 600));
		Time = 0;
	}
	FileTime += DeltaTime;
	if (FileTime > 10)
	{
		WriteFileNameNum += 1;
		FileTime = 0;
	}
	if (WriteOrRead)
	{
		SetFCharacterData();
	}
	else
	{
		ReadFCharacter();
	}
}

void APythonAICharacter::BeginPlay()
{
	Super::BeginPlay();
	FinishLevel.AddDynamic(this, &APythonAICharacter::ResetInterface);
	if (this&& CaptureComponent2D)
	{
		this->DefaultLocation = GetActorLocation();
		this->DefaultRotation = GetActorRotation();
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

void APythonAICharacter::ResetInterface(const FVector& Location, const FString& MethodName, const FString& Args)
{
	if (this)
	{
		SetActorLocationAndRotation(DefaultLocation, DefaultRotation);
		this->ReadWriteSwitch = false;
	}
	CallPyCharacterMethod(MethodName, Args);
}

FString APythonAICharacter::GetPyhtonMethodName()
{
	return m_MethodName;
}

FString APythonAICharacter::GetPyhtonArgs()
{
	return m_Args;
}

void APythonAICharacter::SetFCharacterData()
{
	if (this)
	{
		FVector CharacterLocation = this->GetActorLocation();
		FRotator CharacterRotator = this->GetActorRotation();
		FVector CameraForwardVector(0, 0, 0);
		if (this->FollowCamera)
		{
//			CharacterData.CameraLocation = this->CaptureComponent2D->GetWorldLocation();
			CameraForwardVector = this->FollowCamera ->GetForwardVector();
		}
		FCharacterData CharacterData = FCharacterData(CharacterLocation, CharacterRotator, CameraForwardVector);
		WriteFCharacterData(CharacterData);
		this->CharacterDataArr.Add(CharacterData);
	}
}

void APythonAICharacter::SetPlayerControlledData()
{
	if (InputComponent)
	{
		float MoveFB = InputComponent->GetAxisValue("MoveForward");
		float MoveLR = InputComponent->GetAxisValue("MoveRight");
		float TurnY = InputComponent->GetAxisValue("Turn");
		float TurnX = InputComponent->GetAxisValue("LookUp");
		FPlayerControlledData PlayerControlledData = FPlayerControlledData(MoveFB, MoveLR, TurnY, TurnX);
		this->PlayerControlledDataArr.Add(PlayerControlledData);
	}
}

void APythonAICharacter::ReadPlayerControlledData(const TArray<FPlayerControlledData>& TempPlayerControlledDataArr)
{
	if (this)
	{
		if (TempPlayerControlledDataArr.Num() != 0)
		{
			if (this->Count < TempPlayerControlledDataArr.Num())
			{
				MoveForward(TempPlayerControlledDataArr[Count].MoveFB);
				MoveRight(TempPlayerControlledDataArr[Count].MoveLR);
				TurnAtRate(TempPlayerControlledDataArr[Count].TurnX);
				LookUpAtRate(TempPlayerControlledDataArr[Count].TurnY);
				this->Count += 1;
			}
			else
			{
				this->Count = 0;
				this->ReadWriteSwitch = true;
			}
		}
	}
}

void APythonAICharacter::WriteFCharacterData(const FCharacterData& P_CharacterData)
{
	WriteCharacterLoationData(P_CharacterData);

	WriteCharacterRotator(P_CharacterData);

	WriteCameraForwardData(P_CharacterData);
}

void APythonAICharacter::WriteCharacterLoationData(const FCharacterData& P_CharacterData)
{
	FString CharacterLocationX = FString::SanitizeFloat(P_CharacterData.CharacterLocation.X) + FString("\n");
	FString CharacterLocationY = FString::SanitizeFloat(P_CharacterData.CharacterLocation.Y) + FString("\n");
	FString CharacterLocationZ = FString::SanitizeFloat(P_CharacterData.CharacterLocation.Z) + FString("\n");
	FString TotalCharacterLocationData = CharacterLocationX + CharacterLocationY + CharacterLocationZ;
	UTextReadWrite::SaveText(FString("C:\\Users\\Admin\\Desktop"), FString("CharacterData") + (FString::FromInt(WriteFileNameNum)) + FString(".TXT"), TotalCharacterLocationData);
}

void APythonAICharacter::WriteCharacterRotator(const FCharacterData& P_CharacterData)
{
	FString CharacterForwardX = FString::SanitizeFloat(P_CharacterData.CharacterRotator.Pitch) + FString("\n");
	FString CharacterForwardY = FString::SanitizeFloat(P_CharacterData.CharacterRotator.Yaw) + FString("\n");
	FString CharacterForwardZ = FString::SanitizeFloat(P_CharacterData.CharacterRotator.Roll) + FString("\n");
	FString TotalCharacterRotatorData = CharacterForwardX + CharacterForwardY + CharacterForwardZ;
	UTextReadWrite::SaveText(FString("C:\\Users\\Admin\\Desktop"), FString("CharacterData") + (FString::FromInt(WriteFileNameNum)) + FString(".TXT"), TotalCharacterRotatorData);
}

void APythonAICharacter::WriteCameraForwardData(const FCharacterData& P_CharacterData)
{
	FString CameraForwardX = FString::SanitizeFloat(P_CharacterData.CameraForwardVector.X) + FString("\n");
	FString CameraForwardY = FString::SanitizeFloat(P_CharacterData.CameraForwardVector.Y) + FString("\n");
	FString CameraForwardZ = FString::SanitizeFloat(P_CharacterData.CameraForwardVector.Z) + FString("\n");
	FString TotalCameraForwardData = CameraForwardX + CameraForwardY + CameraForwardZ;
	UTextReadWrite::SaveText(FString("C:\\Users\\Admin\\Desktop"), FString("CharacterData")+(FString::FromInt(WriteFileNameNum))+FString(".TXT"), TotalCameraForwardData);
}

void APythonAICharacter::ReadFCharacter()
{
	if (0 == ReadCharacterDataArr.Num())
	{
		LoadFileSuccess = LoadCharacterDataToStack();
	}
	else if (LoadFileSuccess)
	{
		if (this)
		{

			FVector TempCharacterVector = FVector(FCString::Atof(*ReadCharacterDataArr[LocationCount]), FCString::Atof(*ReadCharacterDataArr[LocationCount + 1]), FCString::Atof(*ReadCharacterDataArr[LocationCount + 2]));
			this->SetActorLocation(TempCharacterVector);
			FRotator TempCharacterRotator = FRotator(FCString::Atof(*ReadCharacterDataArr[LocationCount + 3]), FCString::Atof(*ReadCharacterDataArr[LocationCount + 4]), FCString::Atof(*ReadCharacterDataArr[LocationCount + 5]));
			this->SetActorRotation(TempCharacterRotator);
			this->LocationCount += 9;
			if (this->LocationCount >= ReadCharacterDataArr.Num())
			{
				this->LocationCount = 0;
				this->ReadFileNameNum += 1;
				ReadCharacterDataArr.Empty();
			}
		}
	}
	if(!LoadFileSuccess)
	{
		if (this)
		{
			this->ReadFileNameNum = 0;
		}
	}
}

bool APythonAICharacter::LoadCharacterDataToStack()
{
		bool LoadSuccess = UTextReadWrite::LoadText(FString("C:\\Users\\Admin\\Desktop"), FString("CharacterData") + FString::FromInt(ReadFileNameNum) + FString(".TXT"), ReadCharacterDataArr);
		return LoadSuccess;
}
