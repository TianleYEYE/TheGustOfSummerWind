// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SW_ScriptManager.h"

#include "Game/SW_GameMode.h"
#include "Kismet/GameplayStatics.h"


ASW_ScriptManager::ASW_ScriptManager()
{
	PrimaryActorTick.bCanEverTick = false;

	AudioPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("CustomAudio1"));
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomScene"));
	ConversationalVoicePlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("CustomAudio2"));

	RootComponent = SceneComponent;
	AudioPlayer->SetupAttachment(SceneComponent);
	ConversationalVoicePlayer->SetupAttachment(SceneComponent);
}

void ASW_ScriptManager::BeginPlay()
{
	Super::BeginPlay();

	LoadData();
	InitializeDataTableIndexMap();
}

void ASW_ScriptManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASW_ScriptManager::SetDataTable(UDataTable* EnterDataTable)
{
	DataTable = EnterDataTable;
}

void ASW_ScriptManager::LoadData()
{
	const ASW_GameMode* GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	if (!GameMode)
	{
		return;
	}

	const USW_SystemData* SaveObject = GameMode->GetCGSlotData();
	if (!SaveObject)
	{
		return;
	}

	TextDisplaySpeed = SaveObject->TextDisplaySpeed;
	MasterVolume = SaveObject->MasterVolume;
	SoundEffectVolume = SaveObject->SoundEffectVolume;
	ConversationalVoice = SaveObject->ConversationalVoice;
}

void ASW_ScriptManager::SetRowDialog(int EnterRowDialog)
{
	rowDialog = EnterRowDialog;
}

int ASW_ScriptManager::GetMaxDialogIndex()
{
	if (DataTable)
	{
		MaxDialogIndex = DataTable->GetRowMap().Num();
	}

	return MaxDialogIndex;
}

void ASW_ScriptManager::MenuMusicPlay()
{
	AudioPlayer->SetSound(BackgroundMusic);
	AudioPlayer->Play();
}

void ASW_ScriptManager::InitializeDataTableIndexMap()
{
	DataTableIndexMap.Reset();
	DataTableArray.Reset();

	if (ChapterDataTables.IsEmpty())
	{
		ChapterDataTables.Reserve(MaxChapterCount);
		for (int32 ChapterIndex = 1; ChapterIndex <= MaxChapterCount; ++ChapterIndex)
		{
			ChapterDataTables.Add(TSoftObjectPtr<UDataTable>(FSoftObjectPath(FString::Printf(TEXT("/Game/DialogData/DT_Chapter_%d.DT_Chapter_%d"), ChapterIndex, ChapterIndex))));
		}
	}

	for (int32 Index = 0; Index < ChapterDataTables.Num(); ++Index)
	{
		UDataTable* LoadedTable = ChapterDataTables[Index].LoadSynchronous();
		if (!LoadedTable)
		{
			continue;
		}

		DataTableIndexMap.Add(LoadedTable->GetFName(), Index);
		DataTableArray.Add(LoadedTable);
	}
}

FDialogStruct* ASW_ScriptManager::GetDialogStruct()
{
	if (!DataTable || rowDialog >= GetMaxDialogIndex())
	{
		return nullptr;
	}

	DialogStruct = DataTable->FindRow<FDialogStruct>(DataTable->GetRowNames()[rowDialog], TEXT(""));
	return DialogStruct;
}

FDialogStruct* ASW_ScriptManager::GetPreviousDialogStruct()
{
	if (!DataTable || rowDialog < 1)
	{
		return nullptr;
	}

	PreviousDialogStruct = DataTable->FindRow<FDialogStruct>(DataTable->GetRowNames()[rowDialog - 1], TEXT(""));
	return PreviousDialogStruct;
}

int32 ASW_ScriptManager::GetDataTableIndex(UDataTable* InDataTable)
{
	if (InDataTable && DataTableIndexMap.Contains(InDataTable->GetFName()))
	{
		return DataTableIndexMap[InDataTable->GetFName()];
	}

	return -1;
}

UDataTable* ASW_ScriptManager::GetDataTableByIndex(int32 Index)
{
	if (DataTableArray.IsValidIndex(Index))
	{
		return DataTableArray[Index];
	}

	return nullptr;
}
