
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "DataStruct\DialogStruct.h"
#include "SW_ScriptManager.generated.h"


UENUM(BlueprintType)
enum EWidgetStatus
{
	Menu,
	InGame,
	InGameSaveOrLoad,
	SaveOrLoad,
	ContinueUI,
	AlbumUI,
	SettingUI,
	InGameSetting,
	BackLog,
};

UCLASS()
class THEGUSTOFSUMMERWIND_API ASW_ScriptManager : public AActor
{
	GENERATED_BODY()

	static constexpr int32 MaxChapterCount = 15;

public:
	// Sets default values for this actor's properties
	ASW_ScriptManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Audio")
	TObjectPtr<UAudioComponent>AudioPlayer;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Audio")
	TObjectPtr<UAudioComponent>ConversationalVoicePlayer;

	UPROPERTY( BlueprintReadOnly, Category = "MySceneComponent")
	TObjectPtr<USceneComponent>SceneComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Chapter DataTable")
	TObjectPtr<UDataTable>DataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chapter DataTable")
	TArray<TSoftObjectPtr<UDataTable>> ChapterDataTables;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="properties")
	int rowDialog=0;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="properties")
	int MaxDialogIndex;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="properties")
	TObjectPtr<USoundBase> BackgroundMusic;

	void SetDataTable(UDataTable* EnterDataTable);
	UDataTable* GetDataTable() const {return DataTable;}
	

	UFUNCTION(BlueprintCallable)
	void LoadData();

	UFUNCTION(BlueprintCallable, Category = "System Settings")
	void ApplyRuntimeSettings();

	UFUNCTION(BlueprintCallable, BlueprintSetter, Category = "System Settings")
	void SetTextDisplaySpeed(float InTextDisplaySpeed);

	UFUNCTION(BlueprintCallable, BlueprintSetter, Category = "System Settings")
	void SetMasterVolume(float InMasterVolume);

	UFUNCTION(BlueprintCallable, BlueprintSetter, Category = "System Settings")
	void SetSoundEffectVolume(float InSoundEffectVolume);

	UFUNCTION(BlueprintCallable, BlueprintSetter, Category = "System Settings")
	void SetConversationalVoice(float InConversationalVoice);

	void SetRowDialog(int EnterRowDialog);
	
	int GetRowDialog() const {return rowDialog ;}

	UFUNCTION(BlueprintCallable,BlueprintPure)
	int GetMaxDialogIndex();

	UFUNCTION(BlueprintCallable)
	void MenuMusicPlay();

	void InitializeDataTableIndexMap();
	
	FDialogStruct *GetDialogStruct();
	FDialogStruct *GetPreviousDialogStruct();
	
	FDialogStruct *DialogStruct;
	
	FDialogStruct *PreviousDialogStruct;
	
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EWidgetStatus> WidgetState = Menu;

	UPROPERTY()
	int CurrentChapter;

	UFUNCTION(BlueprintCallable)
	int32 GetDataTableIndex(UDataTable* InDataTable);
	UFUNCTION(BlueprintCallable)
	UDataTable* GetDataTableByIndex(int32 Index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetTextDisplaySpeed, Category = "System Settings")
	float TextDisplaySpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetMasterVolume, Category = "System Settings")
	float MasterVolume = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetSoundEffectVolume, Category = "System Settings")
	float SoundEffectVolume = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetConversationalVoice, Category = "System Settings")
	float ConversationalVoice = 0.5f;


	UFUNCTION(BlueprintCallable)
	TMap<FName, int32> GetDataTableIndexMap()const {return DataTableIndexMap;};

	UFUNCTION(BlueprintCallable)
	TArray<UDataTable*>	GetDataTableArray() const {return DataTableArray;}
	
private:
	TMap<FName, int32> DataTableIndexMap;

	// 声明一个数组来存储DataTable指针
	TArray<UDataTable*> DataTableArray;


	
};
