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
	FRotator CharacterRotator;
	//UPROPERTY(BlueprintReadOnly)
	//FVector CameraLocation;
	UPROPERTY(BlueprintReadOnly)
	FVector CameraForwardVector;
	FCharacterData(){}
	FCharacterData(FVector _CharacterLocation,FRotator _CharacterRotator,FVector _CameraForwardVector):CharacterLocation(_CharacterLocation),CharacterRotator(_CharacterRotator),CameraForwardVector(_CameraForwardVector){}
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
	float TurnX;
	UPROPERTY(BlueprintReadOnly)
	float TurnY;
	UPROPERTY(BlueprintReadOnly)
		int32 Jump;

	FPlayerControlledData(){}
	FPlayerControlledData(float _MoveFB,float _MoveLR, float _TurnX, float _TurnY , int32 _Jump) :MoveFB(_MoveFB), MoveLR(_MoveLR), TurnX(_TurnX), TurnY(_TurnY),Jump(_Jump) {}
	       
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



	UPROPERTY(BlueprintReadWrite)
		TArray<FPlayerControlledData> PlayerControlledDataArraaaa;





	/*记录玩家操作信息*/
	UFUNCTION()
		void SetFPlayerControlledData();

	/*将玩家控制器总信息写入文件*/
	UFUNCTION()
		void WriteFPlayerControlledData(const FPlayerControlledData& P_PlayerControlledData);

	/*记录玩家输入方向键及鼠标方向及跳跃信息*/
	UFUNCTION()
		void WritePlayerControlledBaseData(const FPlayerControlledData& P_PlayerControlledData);

	/*读取数据信息并表现出来*/
	UFUNCTION(BlueprintCallable)
		void ReadFPlayerControlled();



	/*将完成关卡的表象总信息写入文件*/
	UFUNCTION(BlueprintCallable)
		void WriteFCharacterData(const FCharacterData& P_CharacterDataArr);

	/*将完成关卡的表象角色位置信息写入文件*/
	UFUNCTION(BlueprintCallable)
		void WriteCharacterLoationData(const FCharacterData& P_CharacterData);

	/*将完成关卡的表象角色旋转信息写入文件*/
	UFUNCTION(BlueprintCallable)
		void WriteCharacterRotator(const FCharacterData& P_CharacterData);

	/*将完成关卡的表象相机向前向量信息写入文件*/
	UFUNCTION(BlueprintCallable)
		void WriteCameraForwardData(const FCharacterData& P_CharacterData);

	/*读取出角色信息的数组，每个元素中有一个数*/
	TArray<FString> ReadCharacterDataArr;

	/*读取出玩家控制器信息的数组，每个元素中有一个数*/
	TArray<FString> ReadPlayerControlledDataArr;

	/*读写切换，true为写，false为读*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool WriteOrRead = true;

	/*读取数据信息并表现出来*/
	UFUNCTION(BlueprintCallable)
		void ReadFCharacter();

	/*加载文件中角色数据到栈区*/
	UFUNCTION(BlueprintCallable)
		bool LoadCharacterDataToStack();

	/*加载文件中玩家操作数据到栈区*/
	UFUNCTION(BlueprintCallable)
		bool LoadPlayerControlledDataToStack();
	
	/*用于判断文件加载是否成功*/
	UPROPERTY()
		bool LoadFileSuccess = true;

	/*用于记录玩家按下跳跃按键*/
	UFUNCTION()
		void WantToJump();

	/*用于记录是否按下跳跃键*/
	UPROPERTY()
		bool JumpSwitch = false;

	UFUNCTION(BlueprintCallable)
		TArray<FVector>& GetPointFromStartToEnd(const FVector& StartLocation, const FVector& EndLocation);

	UPROPERTY(BlueprintReadWrite)
		TArray<FVector> PathPointsArr;
	UPROPERTY()
		TArray<FVector> EmptyFVectorArr;

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
	float SamplingTime = 0;

	/*用于设置数据写入间隔*/
	float Time1 = 0;

	/*用于记录读取玩家输入信息的限制次数*/
	int32 Count = 0;

	/*每帧设置角色变换的计数*/
	int32 LocationCount = 0;

	/*从文件中读取玩家操作数据的位置*/
	int32 PlayerControlledIndex = 9;

	/*文件切割的时间*/
	float FileTime = 0;

	/*文件写入的数字名称*/
	int32 WriteFileNameNum = 0;

	/*文件读出的数字名称*/
	int32 ReadFileNameNum = 0;

	/*文件读出玩家操作的数字名称*/
	int32 ReadPlayerNameNum = 0;

	/*角色开始的位置*/
	FVector DefaultLocation = FVector(0, 0, 0);

	/*角色开始的旋转*/
	FRotator DefaultRotation = FRotator(0, 0, 0);

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

	/*写入文件的路径*/
	UPROPERTY(BlueprintReadWrite)
		FString WriteCharacterDataTextPath = "C:\\Users\\Admin\\Desktop";
	/*写入文件名*/
	UPROPERTY(BlueprintReadWrite)
		FString WriteCharacterDataTextName = "CharacterData.txt";
	/*读取文件路径*/
	UPROPERTY(BlueprintReadWrite)
		FString ReadCharacterDataTextPath = "C:\\Users\\Admin\\Desktop";
	/*读取文件名*/
	UPROPERTY(BlueprintReadWrite)
		FString ReadCharacterDataTextName = "CharacterData.txt";
	/*初始控制器旋转*/
	UPROPERTY()
		FRotator DefaultControlledRotation = FRotator(0, 0, 0);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool EnableReadWrite = true;


private:

	/*控制器指针*/
	UPROPERTY()
		APlayerController* m_PlayerControllerPtr = nullptr;

	/*世界指针*/
	UPROPERTY()
		UWorld* m_WorldPtr = nullptr;
		
};

