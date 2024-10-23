#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_Next.generated.h"

DECLARE_MULTICAST_DELEGATE(FEntrustNext)

UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_Next : public UBTN_ButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UButton>BTN_Next;

	virtual void NativeConstruct() override;

	//创建委托实例
	FEntrustNext Next;
	//多播委托代理
	UFUNCTION()
	void DelegatedAgent();
};
