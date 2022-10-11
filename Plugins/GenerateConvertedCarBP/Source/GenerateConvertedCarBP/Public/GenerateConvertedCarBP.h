// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

#define PLUGIN_NAME "GenerateConvertedCarBP"

class FGenerateConvertedCarBPModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:

	void	RegisterMenus();
	bool	GenerateBlueprint();

	TSharedPtr<class FUICommandList> PluginCommands;
};
