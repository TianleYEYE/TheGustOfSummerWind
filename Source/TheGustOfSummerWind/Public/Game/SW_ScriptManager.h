
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

	UPROPERTY(BlueprintReadWrite)
	float TextDisplaySpeed = 0.1f;

	UPROPERTY(BlueprintReadWrite)
	float MasterVolume = 0.5f;

	UPROPERTY(BlueprintReadWrite)
	float SoundEffectVolume = 0.5f;

	UPROPERTY(BlueprintReadWrite)
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
