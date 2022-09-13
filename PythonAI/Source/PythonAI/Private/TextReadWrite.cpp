// Fill out your copyright notice in the Description page of Project Settings.


#include "TextReadWrite.h"
#include "Misc/FileHelper.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "HAL/PlatformFilemanager.h"

void UTextReadWrite::SaveText(FString Directory, FString FileName, FString Text)
{
	Directory += "\\"+ FileName;
	FFileHelper::SaveStringToFile(Text, *Directory,FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

void UTextReadWrite::LoadText(FString Directory, FString FileName, TArray<FString>& ResultTextArr)
{
	Directory += "\\" + FileName;
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*Directory))
	{
		FFileHelper::LoadFileToStringArray(ResultTextArr, *Directory);
	}
}
