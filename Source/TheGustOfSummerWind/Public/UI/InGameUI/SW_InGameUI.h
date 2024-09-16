#pragma once

#include "CoreMinimal.h"
#include "SW_DialogBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/SW_UIBase.h"
#include "UI/Button/BTN_ButtonInGame/BTN_Button0fInGameUI.h"
#include "UI/Button/BTN_ButtonInGame/BTN_NextDialog.h"
#include "UI/Button/BTN_ButtonInMenu/BTN_SaveOrRead.h"
#include "DataStruct\DialogStruct.h"
#include "TimerManager.h"
#include "Components/CanvasPanelSlot.h"
#include "Game/SW_GameInstance.h"
#include "Game/SW_ScriptManager.h"
#include "SW_InGameUI.generated.h"


UCLASS()
class THEGUSTOFSUMMERWIND_API USW_InGameUI : public USW_UIBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBTN_Button0fInGameUI>BP_ButtonOfInGameUI;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBTN_SaveOrRead>BP_SaveOrRead;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UTextBlock>TextBlock_Name;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UImage>TEX_Background;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UImage>TEX_CharacterPortraits_1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UImage>TEX_CharacterPortraits_2;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UImage>TEX_CharacterPortraits_3;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USW_DialogBox>BP_DialogBox;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBTN_NextDialog>BP_NextDialog;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBorder>NameBoxBackground;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBorder>InGameMenuUI;

	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>DisplaysInGameMenu;
	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>CancelDisplaysInGameMenu;
	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>One;
	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>Two;
	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>Three;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AudioPlayer")
	TObjectPtr<ASW_ScriptManager>ScriptManager;

	UPROPERTY()
	TObjectPtr<USW_GameInstance>SW_GameInstance;


	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot>ScreenWhiteSlot;
	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot>inGameMenuUISlot;

	FWidgetAnimationDynamicEvent CancelInGameMenuUIEvent;
	FWidgetAnimationDynamicEvent InGameMenuUIEvent;

	
	UFUNCTION()
	void InitializeGame();
	
	UFUNCTION()
	void PressBTN_NEXT(int InRow);
	
	void ReadDialog();
	
	UFUNCTION()
	void GetCGOrStart();
	
	//设置对话角色姓名
	void SetName(FDialogStruct *dialogRow);
	//设置立绘
	void SetCharacterPortraits(FDialogStruct *dialogRow);
	//设置背景
	void SetBackground(FDialogStruct *dialogRow);
	//设置音乐
	void SetMusic(FDialogStruct *dialogRow);
	//设置角色语音
	void SetConversationalVoice(FDialogStruct *dialogRow);
	// //设置CG
	// void SetGameCG(FDialogStruct *dialogRow,int row);
	//设置游戏内菜单
	UFUNCTION()
	void GetInGameMenuUI();
	UFUNCTION()
	void CancelInGameMenuUIImplement();
	
	FDialogStruct *PreviousDialogRow;
	
	int CurrentIndex;
	int Num_nextDialogButtonClicked=0;

	bool bIsWhiteOver=false;
	bool bIsAllAnimationsPlayOver=false;


public:
	int row ;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Dialog Data")
	TObjectPtr<UDataTable>DialogDataTable;

	FDialogStruct *DialogStruct;
	
};
