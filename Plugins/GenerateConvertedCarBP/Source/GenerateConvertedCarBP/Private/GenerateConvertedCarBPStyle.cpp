// Copyright Epic Games, Inc. All Rights Reserved.

#include "GenerateConvertedCarBPStyle.h"
#include "GenerateConvertedCarBP.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FGenerateConvertedCarBPStyle::StyleInstance = nullptr;

void FGenerateConvertedCarBPStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FGenerateConvertedCarBPStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FGenerateConvertedCarBPStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("GenerateConvertedCarBPStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FGenerateConvertedCarBPStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("GenerateConvertedCarBPStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("GenerateConvertedCarBP")->GetBaseDir() / TEXT("Resources"));

	Style->Set("GenerateConvertedCarBP.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FGenerateConvertedCarBPStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FGenerateConvertedCarBPStyle::Get()
{
	return *StyleInstance;
}
