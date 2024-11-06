// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SW_ScriptManager.h"

#include "Game/SW_GameMode.h"
#include "Game/SW_HUD.h"
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

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,FString::Printf(TEXT("TextDisplaySpeed : %f"),MasterVolume));
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


FDialogStruct* ASW_ScriptManager::SetDialogStruct()
{
	if (rowDialog<GetMaxDialogIndex())
	{
		DialogStruct = DataTable->FindRow<FDialogStruct>(DataTable->GetRowNames()[rowDialog],TEXT(""));
		if (rowDialog >= 1)
		{
			PreviousDialogStruct =DataTable->FindRow<FDialogStruct>(DataTable->GetRowNames()[rowDialog-1],TEXT(""));
		}
		return DialogStruct;
	}
	return DialogStruct;
}




