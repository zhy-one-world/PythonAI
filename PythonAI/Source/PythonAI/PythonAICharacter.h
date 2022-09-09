// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameUserSettings.h"
#include "PyCharacter.h"
#include "PythonAICharacter.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFinishLevel, const FVector&, Location,const FString&,MethodName,const FString&,Args);

USTRUCT(Blueprintable, BlueprintType)
struct FMyStruct
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadOnly)
		float HP = 10;
	UPROPERTY(BlueprintReadOnly)
		float Armor = 100;
};

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(BlueprintReadOnly)
	FVector CharacterLocation;
	UPROPERTY(BlueprintReadOnly)
	FVector CharacterForwardVector;
	//UPROPERTY(BlueprintReadOnly)
	//FVector CameraLocation;
	UPROPERTY(BlueprintReadOnly)
	FVector CameraForwardVector;
	FCharacterData(){}
	FCharacterData(FVector _CharacterLocation,FVector _CharacterForwardVector,FVector _CameraForwardVector):CharacterLocation(_CharacterLocation),CharacterForwardVector(_CharacterForwardVector),CameraForwardVector(_CameraForwardVector){}
};
USTRUCT(BlueprintType)
struct FPlayerControlledData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadOnly)
	float MoveFB;
	UPROPERTY(BlueprintReadOnly)
	float MoveLR;
	UPROPERTY(BlueprintReadOnly)
	float TurnY;
	UPROPERTY(BlueprintReadOnly)
	float TurnX;

	FPlayerControlledData(){}
	FPlayerControlledData(float _MoveFB,float _MoveLR, float _TurnY, float _TurnX) :MoveFB(_MoveFB), MoveLR(_MoveLR), TurnY(_TurnY), TurnX(_TurnY) {}
	       
};

UCLASS(config=Game)
class APythonAICharacter : public APyCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Capture, meta = (AllowPrivateAccess = "true"))
		class USceneCaptureComponent2D* CaptureComponent2D;
public:
	
	APythonAICharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/*获取小白人速度*/
	UFUNCTION(BlueprintCallable)
		float GetActorSpeed();

	/*获取抓拍镜头向前向量*/
	UFUNCTION(BlueprintCallable)
		FVector GetCameraForwardVector();

	/*完成关卡的回调*/
	UPROPERTY(BlueprintAssignable)
		FFinishLevel FinishLevel;

	/*重置小白人的设置，包括执行python中的函数*/
	UFUNCTION()
		void ResetInterface(const FVector& Location,const FString& MethodName, const FString& Args);

	/*获取完成关卡要执行python函数的名称*/
	UFUNCTION()
		FString GetPyhtonMethodName();

	/*获取完成关卡要执行python函数的参数*/
	UFUNCTION()
		FString GetPyhtonArgs();

	/*记录完成关卡的表象信息*/
	UPROPERTY(BlueprintReadWrite)
	TArray<FCharacterData> CharacterDataArr;

	/*记录完成关卡的表象信息函数*/
	UFUNCTION()
		void SetFCharacterData();

	/*记录角色完成关卡的操作*/
	UPROPERTY(BlueprintReadWrite)
		TArray<FPlayerControlledData> PlayerControlledDataArr;

	/*记录玩家操作数据函数*/
	UFUNCTION()
		void SetPlayerControlledData();

	/*读取玩家操作*/
	UFUNCTION()
		void ReadPlayerControlledData(const TArray<FPlayerControlledData>& TempPlayerControlledDataArr);
protected:

	void OnResetVR();

	UFUNCTION(BlueprintCallable, Category = Character)
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable, Category = Character)
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
	void TurnAtRate(float Rate);

	UFUNCTION(BlueprintCallable)
	void LookUpAtRate(float Rate);

	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*获取RGBA数组*/
	UFUNCTION(BlueprintCallable)
		TArray<FColor> PictureSampling(const FVector2D& RangeSize);

	/*获取RGBA数组，Python的接口*/
	UPROPERTY(BlueprintReadOnly)
	TArray<FColor> ColorDateArr;

	/*获取RGBA数组*/
	UFUNCTION(BlueprintCallable)
		TArray<FColor> GetColorDateArr(const TArray<FColor>& ColorArr);

	/*目的为了获取屏幕分辨率*/
	UGameUserSettings* UserSetting = nullptr;

	/*世界指针*/
	UWorld* MyWorld = nullptr;

	/*限制获取RGBA间隔时间*/
	float Time = 0;
	float Time1 = 0;
	int32 i = 0;

	/*玩家操纵读写开关*/
	bool ReadWriteSwitch = true;

	/*角色开始的位置*/
	FVector DefaultLocation;

	/*角色开始的旋转*/
	FRotator DefaultRotation;

	/*分辨率信息*/
	FIntPoint Resolution;

	/*在python中测试的结构体*/
	UPROPERTY(BlueprintReadOnly)
		FMyStruct My = { 11.0f,12.0f };

	/*开放在编辑器中的Python方法，在完成关卡调用的python函数*/
	UPROPERTY(EditAnywhere, Category = "LevelFinishPythonMethodName")
		FString m_MethodName;

	/*开放在编辑器中的Python方法的参数，在完成关卡调用的python函数的参数*/
	UPROPERTY(EditAnywhere,Category = "LevelFinishPythonMethodName")
		FString m_Args;
};

