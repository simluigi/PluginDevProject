// Copyright Epic Games, Inc. All Rights Reserved.

#include "GenerateConvertedCarBP.h"
#include "GenerateConvertedCarBPStyle.h"
#include "GenerateConvertedCarBPCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

// For Blueprint generation
#include "Kismet2/KismetEditorUtilities.h"
#include <AssetRegistry/AssetRegistryModule.h>
#include <ObjectTools.h>

// For UAssetImportData
#include "EditorFramework/AssetImportData.h"

// For Skeletal Mesh support
#include "Components/SkeletalMeshComponent.h"

// For Chaos Vehicle support
#include "ChaosVehicles/Public/WheeledVehiclePawn.h"
#include <ChaosVehicleMovementComponent.h>

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
	GenerateBlueprint();
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

bool FGenerateConvertedCarBPModule::GenerateBlueprint()
{
	// Generate blank blueprint of parent type WheeledVehiclePawn	
	FName BlueprintName = FName(ObjectTools::SanitizeObjectName("BP_ConvertTest"));
	FString assetPath = "/" + BlueprintName.ToString();
	UPackage* package = CreatePackage(PackageName);

	UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(
		AWheeledVehiclePawn::StaticClass(),
		package,
		BlueprintName,
		BPTYPE_Normal,
		UBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass()
	);

	if (Blueprint) 
	{
		// Use ConvertTest asset as mesh data for Blueprint
		USkeletalMesh* Mesh = LoadObject<USkeletalMesh>(NULL, TEXT("/Game/ConvertedCar/ConvertTest"), NULL, LOAD_None, NULL);

		// Editing the Blueprint
		AWheeledVehiclePawn* BlueprintValues = Blueprint->GeneratedClass.Get()->GetDefaultObject<AWheeledVehiclePawn>();
		BlueprintValues->GetMesh()->SetSkeletalMesh(Mesh);
		
		// TODO: Set the Mesh's Animation BP
		// BlueprintValues->GetMesh()->SetAnimInstanceClass(Anim_ConvertTest);

		// Compile blueprint
		FKismetEditorUtilities::CompileBlueprint(Blueprint, EBlueprintCompileOptions::None, nullptr);

		// Notify the asset registry
		FAssetRegistryModule::AssetCreated(Blueprint);

		// Mark the package dirty.
		package->MarkPackageDirty();

		return true;
	}

	return false;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGenerateConvertedCarBPModule, GenerateConvertedCarBP)