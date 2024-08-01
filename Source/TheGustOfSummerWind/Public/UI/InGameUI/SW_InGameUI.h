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
#include "TimerManager.h"
#include "Components/CanvasPanelSlot.h"
#include "SW_InGameUI.generated.h"



class AAudioPlayer;


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
	TObjectPtr<UImage>GameCG;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USW_DialogBox>BP_DialogBox;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBTN_NextDialog>BP_NextDialog;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBorder>NameBoxBackground;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	// TObjectPtr<UBorder>ScreenWhite;
	
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
	
	// UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	// TObjectPtr<UWidgetAnimation>ScreenToWhite;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AudioPlayer")
	TObjectPtr<AAudioPlayer>AudioPlayer;


	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot>ScreenWhiteSlot;
	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot>GameCGSlot;
	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot>inGameMenuUISlot;

	FWidgetAnimationDynamicEvent CancelInGameMenuUIEvent;
	FWidgetAnimationDynamicEvent SwitchCG;
	FWidgetAnimationDynamicEvent DialogAnimationSwitch;
	
	UFUNCTION()
	void Delegated();
	
	UFUNCTION()
	void PressBTN_NEXT(uint32& InRow);
	UFUNCTION()
	// void UpdateRowDialog();
	
	void ReadDialog();
	
	UFUNCTION()
	void GetCGOrStart();
	
	
	//设置对话
	void SetDialog();
	void UpdateText();
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
	void GetInGameMenuUI();
	void CancelInGameMenuUI();
	UFUNCTION()
	void CancelInGameMenuUIImplement();
	
	FDialogStruct *PreviousDialogRow;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="properties")
	int maxRow=10;
	
	int Num_nextDialogButtonClicked=0;

	bool bIsWhiteOver=false;

	bool bIsAllAnimationsPlayOver=false;

	TArray<FString>MessageCharactArray;
	int CurrentIndex = 0;
	
	
};
