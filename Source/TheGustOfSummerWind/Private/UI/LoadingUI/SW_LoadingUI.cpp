#include "UI/LoadingUI/SW_LoadingUI.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

void USW_LoadingUI::NativeConstruct()
{
	BuildFallbackLayoutIfNeeded();
	Super::NativeConstruct();
}

void USW_LoadingUI::BuildFallbackLayoutIfNeeded()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UCanvasPanel* RootPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("LoadingRoot"));
	WidgetTree->RootWidget = RootPanel;

	UBorder* LoadingBackground = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("LoadingBackground"));
	LoadingBackground->SetBrushColor(FLinearColor::Black);

	if (UCanvasPanelSlot* BackgroundSlot = RootPanel->AddChildToCanvas(LoadingBackground))
	{
		BackgroundSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		BackgroundSlot->SetOffsets(FMargin(0.0f));
	}

	UTextBlock* LoadingText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("LoadingText"));
	LoadingText->SetText(FText::FromString(TEXT("Loading...")));
	LoadingText->SetJustification(ETextJustify::Center);
	LoadingText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	LoadingBackground->SetContent(LoadingText);
}
