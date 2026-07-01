#pragma once

#include "CoreMinimal.h"
#include "UI/SW_UIBase.h"
#include "SW_LoadingUI.generated.h"

class UWidgetAnimation;

UCLASS()
class THEGUSTOFSUMMERWIND_API USW_LoadingUI : public USW_UIBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> Fade;

protected:
	virtual UWidgetAnimation* GetFadeAnimation() const override { return Fade; }

private:
	void BuildFallbackLayoutIfNeeded();
};
