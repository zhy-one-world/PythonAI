// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PythonBlueprintFunctionLibrary.h"
#include "TextReadWrite.generated.h"

/**
 * 
 */
UCLASS()
class PYTHONAI_API UTextReadWrite : public UPythonBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
		/*保存文件*/
		UFUNCTION(BlueprintCallable)
		static void SaveText(FString Directory, FString FileName, FString Text);

		/*加载文件*/
		UFUNCTION(BlueprintCallable)
			static bool LoadText(FString Directory, FString FileName, TArray<FString>&ResultTextArr);
};
