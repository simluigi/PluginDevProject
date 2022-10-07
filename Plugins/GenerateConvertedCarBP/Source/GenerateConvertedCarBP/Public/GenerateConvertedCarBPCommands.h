// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "GenerateConvertedCarBPStyle.h"

class FGenerateConvertedCarBPCommands : public TCommands<FGenerateConvertedCarBPCommands>
{
public:

	FGenerateConvertedCarBPCommands()
		: TCommands<FGenerateConvertedCarBPCommands>(TEXT("GenerateConvertedCarBP"), NSLOCTEXT("Contexts", "GenerateConvertedCarBP", "GenerateConvertedCarBP Plugin"), NAME_None, FGenerateConvertedCarBPStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
