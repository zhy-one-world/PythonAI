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

	/*��ȡС�����ٶ�*/
	UFUNCTION(BlueprintCallable)
		float GetActorSpeed();

	/*��ȡץ�ľ�ͷ��ǰ����*/
	UFUNCTION(BlueprintCallable)
		FVector GetCameraForwardVector();

	/*��ɹؿ��Ļص�*/
	UPROPERTY(BlueprintAssignable)
		FFinishLevel FinishLevel;

	/*����С���˵����ã�����ִ��python�еĺ���*/
	UFUNCTION()
		void ResetInterface(const FVector& Location,const FString& MethodName, const FString& Args);

	/*��ȡ��ɹؿ�Ҫִ��python����������*/
	UFUNCTION()
		FString GetPyhtonMethodName();

	/*��ȡ��ɹؿ�Ҫִ��python�����Ĳ���*/
	UFUNCTION()
		FString GetPyhtonArgs();

	/*��¼��ɹؿ��ı�����Ϣ*/
	UPROPERTY(BlueprintReadWrite)
	TArray<FCharacterData> CharacterDataArr;

	/*��¼��ɹؿ��ı�����Ϣ����*/
	UFUNCTION()
		void SetFCharacterData();

	/*��¼��ɫ��ɹؿ��Ĳ���*/
	UPROPERTY(BlueprintReadWrite)
		TArray<FPlayerControlledData> PlayerControlledDataArr;



	UPROPERTY(BlueprintReadWrite)
		TArray<FPlayerControlledData> PlayerControlledDataArraaaa;





	/*��¼��Ҳ�����Ϣ*/
	UFUNCTION()
		void SetFPlayerControlledData();

	/*����ҿ���������Ϣд���ļ�*/
	UFUNCTION()
		void WriteFPlayerControlledData(const FPlayerControlledData& P_PlayerControlledData);

	/*��¼������뷽�������귽����Ծ��Ϣ*/
	UFUNCTION()
		void WritePlayerControlledBaseData(const FPlayerControlledData& P_PlayerControlledData);

	/*��ȡ������Ϣ�����ֳ���*/
	UFUNCTION(BlueprintCallable)
		void ReadFPlayerControlled();



	/*����ɹؿ��ı�������Ϣд���ļ�*/
	UFUNCTION(BlueprintCallable)
		void WriteFCharacterData(const FCharacterData& P_CharacterDataArr);

	/*����ɹؿ��ı����ɫλ����Ϣд���ļ�*/
	UFUNCTION(BlueprintCallable)
		void WriteCharacterLoationData(const FCharacterData& P_CharacterData);

	/*����ɹؿ��ı����ɫ��ת��Ϣд���ļ�*/
	UFUNCTION(BlueprintCallable)
		void WriteCharacterRotator(const FCharacterData& P_CharacterData);

	/*����ɹؿ��ı��������ǰ������Ϣд���ļ�*/
	UFUNCTION(BlueprintCallable)
		void WriteCameraForwardData(const FCharacterData& P_CharacterData);

	/*��ȡ����ɫ��Ϣ�����飬ÿ��Ԫ������һ����*/
	TArray<FString> ReadCharacterDataArr;

	/*��ȡ����ҿ�������Ϣ�����飬ÿ��Ԫ������һ����*/
	TArray<FString> ReadPlayerControlledDataArr;

	/*��д�л���trueΪд��falseΪ��*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool WriteOrRead = true;

	/*��ȡ������Ϣ�����ֳ���*/
	UFUNCTION(BlueprintCallable)
		void ReadFCharacter();

	/*�����ļ��н�ɫ���ݵ�ջ��*/
	UFUNCTION(BlueprintCallable)
		bool LoadCharacterDataToStack();

	/*�����ļ�����Ҳ������ݵ�ջ��*/
	UFUNCTION(BlueprintCallable)
		bool LoadPlayerControlledDataToStack();
	
	/*�����ж��ļ������Ƿ�ɹ�*/
	UPROPERTY()
		bool LoadFileSuccess = true;

	/*���ڼ�¼��Ұ�����Ծ����*/
	UFUNCTION()
		void WantToJump();

	/*���ڼ�¼�Ƿ�����Ծ��*/
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

	/*��ȡRGBA����*/
	UFUNCTION(BlueprintCallable)
		TArray<FColor> PictureSampling(const FVector2D& RangeSize);

	/*��ȡRGBA���飬Python�Ľӿ�*/
	UPROPERTY(BlueprintReadOnly)
	TArray<FColor> ColorDateArr;

	/*��ȡRGBA����*/
	UFUNCTION(BlueprintCallable)
		TArray<FColor> GetColorDateArr(const TArray<FColor>& ColorArr);

	/*Ŀ��Ϊ�˻�ȡ��Ļ�ֱ���*/
	UGameUserSettings* UserSetting = nullptr;

	/*����ָ��*/
	UWorld* MyWorld = nullptr;

	/*���ƻ�ȡRGBA���ʱ��*/
	float SamplingTime = 0;

	/*������������д����*/
	float Time1 = 0;

	/*���ڼ�¼��ȡ���������Ϣ�����ƴ���*/
	int32 Count = 0;

	/*ÿ֡���ý�ɫ�任�ļ���*/
	int32 LocationCount = 0;

	/*���ļ��ж�ȡ��Ҳ������ݵ�λ��*/
	int32 PlayerControlledIndex = 9;

	/*�ļ��и��ʱ��*/
	float FileTime = 0;

	/*�ļ�д�����������*/
	int32 WriteFileNameNum = 0;

	/*�ļ���������������*/
	int32 ReadFileNameNum = 0;

	/*�ļ�������Ҳ�������������*/
	int32 ReadPlayerNameNum = 0;

	/*��ɫ��ʼ��λ��*/
	FVector DefaultLocation = FVector(0, 0, 0);

	/*��ɫ��ʼ����ת*/
	FRotator DefaultRotation = FRotator(0, 0, 0);

	/*�ֱ�����Ϣ*/
	FIntPoint Resolution;

	/*��python�в��ԵĽṹ��*/
	UPROPERTY(BlueprintReadOnly)
		FMyStruct My = { 11.0f,12.0f };

	/*�����ڱ༭���е�Python����������ɹؿ����õ�python����*/
	UPROPERTY(EditAnywhere, Category = "LevelFinishPythonMethodName")
		FString m_MethodName;

	/*�����ڱ༭���е�Python�����Ĳ���������ɹؿ����õ�python�����Ĳ���*/
	UPROPERTY(EditAnywhere,Category = "LevelFinishPythonMethodName")
		FString m_Args;

	/*д���ļ���·��*/
	UPROPERTY(BlueprintReadWrite)
		FString WriteCharacterDataTextPath = "C:\\Users\\Admin\\Desktop";
	/*д���ļ���*/
	UPROPERTY(BlueprintReadWrite)
		FString WriteCharacterDataTextName = "CharacterData.txt";
	/*��ȡ�ļ�·��*/
	UPROPERTY(BlueprintReadWrite)
		FString ReadCharacterDataTextPath = "C:\\Users\\Admin\\Desktop";
	/*��ȡ�ļ���*/
	UPROPERTY(BlueprintReadWrite)
		FString ReadCharacterDataTextName = "CharacterData.txt";
	/*��ʼ��������ת*/
	UPROPERTY()
		FRotator DefaultControlledRotation = FRotator(0, 0, 0);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool EnableReadWrite = true;


private:

	/*������ָ��*/
	UPROPERTY()
		APlayerController* m_PlayerControllerPtr = nullptr;

	/*����ָ��*/
	UPROPERTY()
		UWorld* m_WorldPtr = nullptr;
		
};

