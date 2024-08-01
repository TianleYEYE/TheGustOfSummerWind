#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_NextDialog.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEntrustNextDialog,uint32&)


UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_NextDialog : public UBTN_ButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UButton>BTN_NextDialog;

	virtual void NativeConstruct() override;
	
	//创建委托实例
	FEntrustNextDialog EntrustDelegated;
	//多播委托代理
	UFUNCTION()
	void DelegatedAgent();
	
}; 