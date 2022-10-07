// Copyright Epic Games, Inc. All Rights Reserved.

#include "GenerateConvertedCarBP.h"
#include "GenerateConvertedCarBPStyle.h"
#include "GenerateConvertedCarBPCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

// For file dialog.
#include "Interfaces/IMainFrameModule.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"

// For Blueprint generation
#include "Kismet2/KismetEditorUtilities.h"
#include <AssetRegistry/AssetRegistryModule.h>
#include <ObjectTools.h>

// For Chaos Vehicle support
#include "../../../../../../../../../Program Files/Epic Games/UE_5.0/Engine/Plugins/Experimental/ChaosVehiclesPlugin/Source/ChaosVehicles/Public/WheeledVehiclePawn.h"


static const FName GenerateConvertedCarBPTabName("GenerateConvertedCarBP");
static const TCHAR PackageName[] = TEXT("/Game/Blueprints/BP_ConvertTest");

#define LOCTEXT_NAMESPACE		"FGenerateConvertedCarBPModule"

void FGenerateConvertedCarBPModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FGenerateConvertedCarBPStyle::Initialize();
	FGenerateConvertedCarBPStyle::ReloadTextures();

	FGenerateConvertedCarBPCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGenerateConvertedCarBPCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FGenerateConvertedCarBPModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGenerateConvertedCarBPModule::RegisterMenus));
}

void FGenerateConvertedCarBPModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGenerateConvertedCarBPStyle::Shutdown();

	FGenerateConvertedCarBPCommands::Unregister();
}

void FGenerateConvertedCarBPModule::PluginButtonClicked()
{
	//// select Converted Car output to import
	//FString importPath = "/Game/ConvertedCar/";
	//bool validPath = OpenFileDialog("ConvertTest File | *.uasset", "Select converted car file to import", importPath);
	//if (!validPath)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get file path!"))
	//		return;
	//}
	
	// Generate blank blueprint of parent type WheeledVehiclePawn	
	FName BlueprintName = FName(ObjectTools::SanitizeObjectName("BP_ConvertTest"));
	FString AssetPath = "/" + BlueprintName.ToString();
	UPackage* Package = CreatePackage(PackageName);

	UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(
		AWheeledVehiclePawn::StaticClass(),
		Package,
		BlueprintName,
		BPTYPE_Normal,
		UBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass()
	);

	if (Blueprint) {

		// Editing the Blueprint
		AWheeledVehiclePawn* BlueprintValues = Blueprint->GeneratedClass.Get()->GetDefaultObject<AWheeledVehiclePawn>();
		//BlueprintValues->

		// Compile blueprint
		FKismetEditorUtilities::CompileBlueprint(Blueprint, EBlueprintCompileOptions::None, nullptr);

		// Notify the asset registry
		FAssetRegistryModule::AssetCreated(Blueprint);

		// Mark the package dirty.
		Package->MarkPackageDirty();

		// Show successful asset creation dialog
		
	}

	
}

void FGenerateConvertedCarBPModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FGenerateConvertedCarBPCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FGenerateConvertedCarBPCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

bool FGenerateConvertedCarBPModule::OpenFileDialog(const char* extension, const char* fileDialogName, FString& retPath)
{
	// Get window handle
	void* windowHandle = nullptr;
	if (GIsEditor)
	{
		// Get main window
		IMainFrameModule& mainFrameModule = IMainFrameModule::Get();
		TSharedPtr<SWindow> mainWindow = mainFrameModule.GetParentWindow();

		if (mainWindow.IsValid() && mainWindow->GetNativeWindow().IsValid())
		{
			windowHandle = mainWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}
	else
	{
		if (GEngine && GEngine->GameViewport)
		{
			windowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
		}
	}
	if (windowHandle == nullptr) return false;

	IDesktopPlatform* desktopPlatform = FDesktopPlatformModule::Get();
	if (desktopPlatform == nullptr) return false;

	TArray<FString> FilePaths;
	bool result = desktopPlatform->OpenFileDialog(
		windowHandle,
		fileDialogName,
		TEXT(""),
		TEXT(""),
		extension,
		EFileDialogFlags::Type::None,
		FilePaths);

	if (FilePaths.Num() < 1)	return false;

	retPath = FilePaths[0];
	return true;
}

bool FGenerateConvertedCarBPModule::GenerateBPAsset()
{
	// Create JSON File
	// FString JSONFilePath = 
	return false;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGenerateConvertedCarBPModule, GenerateConvertedCarBP)