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
	ApplySpriteToPortrait(CurrentDialog.Character_1, PreviousDialog.Character_1, 0);
	ApplySpriteToPortrait(CurrentDialog.Character_2, PreviousDialog.Character_2, 1);
	ApplySpriteToPortrait(CurrentDialog.Character_3, PreviousDialog.Character_3, 2);
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

USW_CharacterPortraits* USW_CharacterBox::GetCharacterPortraitById(const FName CharacterId) const
{
	if (const TObjectPtr<USW_CharacterPortraits>* Portrait = ActivePortraitsByCharacterId.Find(CharacterId))
	{
		return Portrait->Get();
	}

	return nullptr;
}

void USW_CharacterBox::ClearCharacterBindings()
{
	ActivePortraitsByCharacterId.Empty();
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

USW_CharacterPortraits* USW_CharacterBox::ResolvePortraitForSprite(const FCharacterSprite& Sprite, const int32 PreferredIndex)
{
	if (!Sprite.CharacterId.IsNone())
	{
		if (USW_CharacterPortraits* ExistingPortrait = GetCharacterPortraitById(Sprite.CharacterId))
		{
			return ExistingPortrait;
		}
	}

	USW_CharacterPortraits* Portrait = FindAvailablePortrait(PreferredIndex);
	if (Portrait && !Sprite.CharacterId.IsNone())
	{
		ActivePortraitsByCharacterId.Add(Sprite.CharacterId, Portrait);
	}

	return Portrait;
}

USW_CharacterPortraits* USW_CharacterBox::FindAvailablePortrait(const int32 PreferredIndex) const
{
	if (USW_CharacterPortraits* PreferredPortrait = GetCharacterPortraitAt(PreferredIndex))
	{
		if (!PreferredPortrait->IsCharacterVisible() || PreferredPortrait->ActiveCharacterId.IsNone())
		{
			return PreferredPortrait;
		}
	}

	for (int32 Index = 0; Index < 3; ++Index)
	{
		USW_CharacterPortraits* Portrait = GetCharacterPortraitAt(Index);
		if (Portrait && (!Portrait->IsCharacterVisible() || Portrait->ActiveCharacterId.IsNone()))
		{
			return Portrait;
		}
	}

	return GetCharacterPortraitAt(PreferredIndex);
}

void USW_CharacterBox::ApplySpriteToPortrait(const FCharacterSprite& CurrentSprite, const FCharacterSprite& PreviousSprite, const int32 PreferredIndex)
{
	const bool bHasCharacterControl =
		!CurrentSprite.CharacterId.IsNone() ||
		CurrentSprite.CurrentSprite ||
		CurrentSprite.Face ||
		CurrentSprite.bHide ||
		!CurrentSprite.bKeepPrevious ||
		CurrentSprite.Position != ECharacterSlotPosition::Inherit;

	if (!bHasCharacterControl)
	{
		if (!CurrentSprite.bKeepPrevious)
		{
			if (USW_CharacterPortraits* Portrait = GetCharacterPortraitAt(PreferredIndex))
			{
				RemoveBindingsForPortrait(Portrait);
				Portrait->HideCharacter(ECharacterTransitionType::None);
			}
		}
		return;
	}

	USW_CharacterPortraits* Portrait = ResolvePortraitForSprite(CurrentSprite, PreferredIndex);
	if (!Portrait)
	{
		return;
	}

	if (CurrentSprite.bHide && !CurrentSprite.CharacterId.IsNone())
	{
		ActivePortraitsByCharacterId.Remove(CurrentSprite.CharacterId);
	}
	else if (CurrentSprite.bHide)
	{
		RemoveBindingsForPortrait(Portrait);
	}

	Portrait->ApplyCharacterSprite(CurrentSprite, PreviousSprite);
}

void USW_CharacterBox::RemoveBindingsForPortrait(USW_CharacterPortraits* Portrait)
{
	if (!Portrait)
	{
		return;
	}

	for (auto It = ActivePortraitsByCharacterId.CreateIterator(); It; ++It)
	{
		if (It.Value() == Portrait)
		{
			It.RemoveCurrent();
		}
	}
}
