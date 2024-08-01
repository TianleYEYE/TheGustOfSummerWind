#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_Previous.generated.h"

DECLARE_MULTICAST_DELEGATE(FEntrustPrevious)

UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_Previous : public UBTN_ButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UButton>BTN_Previous;

	virtual void NativeConstruct() override;

	//创建委托实例
	FEntrustPrevious Previous;
	//多播委托代理
	UFUNCTION()
	void DelegatedAgent();
};
