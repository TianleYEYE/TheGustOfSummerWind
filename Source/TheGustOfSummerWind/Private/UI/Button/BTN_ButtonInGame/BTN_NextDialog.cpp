

#include "UI/Button/BTN_ButtonInGame/BTN_NextDialog.h"


void UBTN_NextDialog::NativeConstruct()
{
	Super::NativeConstruct();
	if (BTN_NextDialog)
	{
		BTN_NextDialog->OnClicked.RemoveDynamic(this, &UBTN_NextDialog::DelegatedAgent);
		BTN_NextDialog->OnClicked.AddUniqueDynamic(this, &UBTN_NextDialog::DelegatedAgent);
	}
	
}


void UBTN_NextDialog::DelegatedAgent()
{
	const ASW_ScriptManager* Manager = GetScriptManager();
	EntrustDelegated.Broadcast(Manager ? Manager->rowDialog : 0);
}
