// Copyright Epic Games, Inc. All Rights Reserved.

#include "GenerateConvertedCarBPCommands.h"

#define LOCTEXT_NAMESPACE "FGenerateConvertedCarBPModule"

void FGenerateConvertedCarBPCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "GenerateConvertedCarBP", "Execute GenerateConvertedCarBP action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
