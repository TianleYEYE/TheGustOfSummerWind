#pragma once

#include "CoreMinimal.h"
#include "SW_CharacterPortraits.h"
#include "SW_DialogBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "UI/SW_UIBase.h"
#include "UI/Button/BTN_ButtonInGame/BTN_NextDialog.h"
#include "DataStruct\DialogStruct.h"
#include "Components/CanvasPanelSlot.h"
#include "SW_InGameUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogueRecord,FText,readText,FText,readName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSetCharacter,FDialogStruct,DialogStruct,FDialogStruct,PreviousDialogStruct);


UCLASS()
class THEGUSTOFSUMMERWIND_API USW_InGameUI : public USW_UIBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UTextBlock>TextBlock_Name;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UImage>TEX_Background;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	// TObjectPtr<USW_CharacterPortraits>BP_CharacterPortraits_0;
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	// TObjectPtr<USW_CharacterPortraits>BP_CharacterPortraits_1;
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	// TObjectPtr<USW_CharacterPortraits>BP_CharacterPortraits_2;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USW_DialogBox>BP_DialogBox;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBTN_NextDialog>BP_NextDialog;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBorder>NameBoxBackground;
	

	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>Fade;

	UPROPERTY(BlueprintAssignable) 
	FSetCharacter SetCharacter;

	
	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot>inGameMenuUISlot;

	FWidgetAnimationDynamicEvent CancelInGameMenuUIEvent;
	FWidgetAnimationDynamicEvent InGameMenuUIEvent;

	bool bIsInGameMenuUIExist =false;
	bool bIsPlayAnimation =false;
	
	UFUNCTION()
	void InitializeGame();
	
	UFUNCTION()
	void PressBTN_NEXT(int InRow);

	UFUNCTION(BlueprintCallable)
	void ReadDialog();
	//设置前后结构体
	void UpdateStruct();
	//设置对话角色姓名
	void SetName(FDialogStruct *dialogRow);
	//设置背景
	void SetBackground(FDialogStruct *dialogRow);
	//设置音乐
	void SetMusic(FDialogStruct *dialogRow);
	//设置角色语音
	void SetConversationalVoice(FDialogStruct *dialogRow);
	//设置切换游戏章节
	UFUNCTION()
	void SwitchChapter();

	UPROPERTY(BlueprintAssignable)
	FDialogueRecord DialogueRecord;
	

	FDialogStruct *PreviousDialogStruct;
	
	int CurrentIndex;
	int Num_nextDialogButtonClicked=0;

	bool bIsWhiteOver=false;

public:
	int row ;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Dialog Data")
	TObjectPtr<UDataTable>DialogDataTable;

	FDialogStruct *DialogStruct;
	
};
