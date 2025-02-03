// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SW_ScriptManager.h"

#include "Game/SW_GameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASW_ScriptManager::ASW_ScriptManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioPlayer=CreateDefaultSubobject<UAudioComponent>(TEXT("CustomAudio1"));
	SceneComponent=CreateDefaultSubobject<USceneComponent>(TEXT("CustomScene"));
	ConversationalVoicePlayer=CreateDefaultSubobject<UAudioComponent>(TEXT("CustomAudio2"));

	RootComponent=SceneComponent;
	AudioPlayer->SetupAttachment(SceneComponent);
	ConversationalVoicePlayer->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void ASW_ScriptManager::BeginPlay()
{
	Super::BeginPlay();

	LoadData();

	InitializeDataTableIndexMap();
}

// Called every frame
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
	
	ASW_GameMode* GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	USW_SystemData* SaveObject = GameMode->GetCGSlotData();

	TextDisplaySpeed = SaveObject -> TextDisplaySpeed;
	MasterVolume = SaveObject ->MasterVolume;
	SoundEffectVolume = SaveObject ->SoundEffectVolume;
	ConversationalVoice = SaveObject ->ConversationalVoice;

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,FString::Printf(TEXT("CurrentChapter : %d"),CurrentChapter));
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
	UDataTable* DataTable_1 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_1.DT_Chapter_1"));
	UDataTable* DataTable_2 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_2.DT_Chapter_2"));
	UDataTable* DataTable_3 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_3.DT_Chapter_3"));
	UDataTable* DataTable_4 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_4.DT_Chapter_4"));
	UDataTable* DataTable_5 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_5.DT_Chapter_5"));
	UDataTable* DataTable_6 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_6.DT_Chapter_6"));
	UDataTable* DataTable_7 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_7.DT_Chapter_7"));
	UDataTable* DataTable_8 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_8.DT_Chapter_8"));
	UDataTable* DataTable_9 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_9.DT_Chapter_9"));
	UDataTable* DataTable_10 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_10.DT_Chapter_10"));
	UDataTable* DataTable_11 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_11.DT_Chapter_11"));
	UDataTable* DataTable_12 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_12.DT_Chapter_12"));
	UDataTable* DataTable_13 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_13.DT_Chapter_13"));
	UDataTable* DataTable_14 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_14.DT_Chapter_14"));
	UDataTable* DataTable_15 = LoadObject<UDataTable>(nullptr,TEXT("/Game/DialogData/DT_Chapter_15.DT_Chapter_15"));
	

	DataTableIndexMap.Add(DataTable_1->GetFName(), 0);
	DataTableIndexMap.Add(DataTable_2->GetFName(), 1);
	DataTableIndexMap.Add(DataTable_3->GetFName(), 2);
	DataTableIndexMap.Add(DataTable_4->GetFName(), 3);
	DataTableIndexMap.Add(DataTable_5->GetFName(), 4);
	DataTableIndexMap.Add(DataTable_6->GetFName(), 5);
	DataTableIndexMap.Add(DataTable_7->GetFName(), 6);
	DataTableIndexMap.Add(DataTable_8->GetFName(), 7);
	DataTableIndexMap.Add(DataTable_9->GetFName(), 8);
	DataTableIndexMap.Add(DataTable_10->GetFName(), 9);
	DataTableIndexMap.Add(DataTable_11->GetFName(), 10);
	DataTableIndexMap.Add(DataTable_12->GetFName(), 11);
	DataTableIndexMap.Add(DataTable_13->GetFName(), 12);
	DataTableIndexMap.Add(DataTable_14->GetFName(), 13);
	DataTableIndexMap.Add(DataTable_15->GetFName(), 14);


	DataTableArray.Add(DataTable_1);
	DataTableArray.Add(DataTable_2);
	DataTableArray.Add(DataTable_3);
	DataTableArray.Add(DataTable_4);
	DataTableArray.Add(DataTable_5);
	DataTableArray.Add(DataTable_6);
	DataTableArray.Add(DataTable_7);
	DataTableArray.Add(DataTable_8);
	DataTableArray.Add(DataTable_9);
	DataTableArray.Add(DataTable_10);
	DataTableArray.Add(DataTable_11);
	DataTableArray.Add(DataTable_12);
	DataTableArray.Add(DataTable_13);
	DataTableArray.Add(DataTable_14);
	DataTableArray.Add(DataTable_15);
	

}


FDialogStruct* ASW_ScriptManager::GetDialogStruct()
{
	if (rowDialog<GetMaxDialogIndex())
	{
		//读表，返回表中内容的结构体
		DialogStruct = DataTable->FindRow<FDialogStruct>(DataTable->GetRowNames()[rowDialog],TEXT(""));
		return DialogStruct;
	}
	return DialogStruct;
}

FDialogStruct* ASW_ScriptManager::GetPreviousDialogStruct()
{
	if (rowDialog >= 1)
	{
		PreviousDialogStruct =DataTable->FindRow<FDialogStruct>(DataTable->GetRowNames()[rowDialog-1],TEXT(""));
	}
	return PreviousDialogStruct;
}

int32 ASW_ScriptManager::GetDataTableIndex(UDataTable* InDataTable)
{
	if (DataTable && DataTableIndexMap.Contains(DataTable->GetFName()))
	{
		return DataTableIndexMap[DataTable->GetFName()];
	}
	return -1; 
}

UDataTable* ASW_ScriptManager::GetDataTableByIndex(int32 Index)
{
	if (DataTableArray.IsValidIndex(Index))
	{
		return DataTableArray[Index];
	}
	return nullptr; // 返回nullptr表示索引无效
}




