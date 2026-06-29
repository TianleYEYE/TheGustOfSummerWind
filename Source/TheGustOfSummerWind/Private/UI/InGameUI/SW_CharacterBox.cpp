#include "UI/InGameUI/SW_CharacterBox.h"

#include "Blueprint/UserWidget.h"
#include "UI/InGameUI/SW_CharacterPortraits.h"

namespace
{
template <typename WidgetType>
WidgetType* FindWidgetByName(UUserWidget* OwnerWidget, const FName WidgetName)
{
	return OwnerWidget ? Cast<WidgetType>(OwnerWidget->GetWidgetFromName(WidgetName)) : nullptr;
}
}

void USW_CharacterBox::NativeConstruct()
{
	Super::NativeConstruct();

	CachePortraitReferences(this);
}

void USW_CharacterBox::InitializeFromOwnerWidget(UUserWidget* OwnerWidget)
{
	CachePortraitReferences(this);

	if (OwnerWidget && OwnerWidget != this)
	{
		CachePortraitReferences(OwnerWidget);
	}
}

void USW_CharacterBox::UpdateCharacters(const FDialogStruct& CurrentDialog, const FDialogStruct& PreviousDialog)
{
	if (BP_CharacterPortraits_0)
	{
		BP_CharacterPortraits_0->UpdateCharacterEvent(CurrentDialog.Character_1, PreviousDialog.Character_1);
	}

	if (BP_CharacterPortraits_1)
	{
		BP_CharacterPortraits_1->UpdateCharacterEvent(CurrentDialog.Character_2, PreviousDialog.Character_2);
	}

	if (BP_CharacterPortraits_2)
	{
		BP_CharacterPortraits_2->UpdateCharacterEvent(CurrentDialog.Character_3, PreviousDialog.Character_3);
	}
}

USW_CharacterPortraits* USW_CharacterBox::GetCharacterPortraitAt(const int32 Index) const
{
	switch (Index)
	{
	case 0:
		return BP_CharacterPortraits_0;
	case 1:
		return BP_CharacterPortraits_1;
	case 2:
		return BP_CharacterPortraits_2;
	default:
		return nullptr;
	}
}

void USW_CharacterBox::CachePortraitReferences(UUserWidget* LegacyOwnerWidget)
{
	if (!BP_CharacterPortraits_0)
	{
		BP_CharacterPortraits_0 = FindWidgetByName<USW_CharacterPortraits>(LegacyOwnerWidget, TEXT("BP_CharacterPortraits_0"));
	}

	if (!BP_CharacterPortraits_0)
	{
		BP_CharacterPortraits_0 = FindWidgetByName<USW_CharacterPortraits>(LegacyOwnerWidget, TEXT("BP_CharacterPortraits"));
	}

	if (!BP_CharacterPortraits_1)
	{
		BP_CharacterPortraits_1 = FindWidgetByName<USW_CharacterPortraits>(LegacyOwnerWidget, TEXT("BP_CharacterPortraits_1"));
	}

	if (!BP_CharacterPortraits_2)
	{
		BP_CharacterPortraits_2 = FindWidgetByName<USW_CharacterPortraits>(LegacyOwnerWidget, TEXT("BP_CharacterPortraits_2"));
	}
}
