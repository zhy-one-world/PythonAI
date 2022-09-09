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

	/*��¼��Ҳ������ݺ���*/
	UFUNCTION()
		void SetPlayerControlledData();

	/*��ȡ��Ҳ���*/
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
	float Time = 0;
	float Time1 = 0;
	int32 i = 0;

	/*��Ҳ��ݶ�д����*/
	bool ReadWriteSwitch = true;

	/*��ɫ��ʼ��λ��*/
	FVector DefaultLocation;

	/*��ɫ��ʼ����ת*/
	FRotator DefaultRotation;

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
};

