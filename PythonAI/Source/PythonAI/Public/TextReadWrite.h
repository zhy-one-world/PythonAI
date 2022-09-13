// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextReadWrite.generated.h"

/**
 * 
 */
UCLASS()
class PYTHONAI_API UTextReadWrite : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
		UFUNCTION(BlueprintCallable)
		static void SaveText(FString Directory, FString FileName, FString Text);

		UFUNCTION(BlueprintCallable)
			static void LoadText(FString Directory, FString FileName, TArray<FString>& ResultTextArr);
};
