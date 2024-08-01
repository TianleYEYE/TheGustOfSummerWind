

#include "UI/Button/BTN_ButtonInGame/BTN_NextDialog.h"


void UBTN_NextDialog::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_NextDialog->OnClicked.AddDynamic(this,&UBTN_NextDialog::DelegatedAgent);
	
}


void UBTN_NextDialog::DelegatedAgent()
{
	uint32 currentRow=0;
	//执行多播函数
	EntrustDelegated.Broadcast(currentRow);
	
}
