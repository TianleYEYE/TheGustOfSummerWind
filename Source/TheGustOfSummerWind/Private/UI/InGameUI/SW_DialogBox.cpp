// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InGameUI/SW_DialogBox.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "UObject/UnrealType.h"

namespace
{
const FName AlphaTextSetDialogTextFunctionNames[] = {
	TEXT("SetDialogText"),
	TEXT("SetText"),
	TEXT("Set Text")
};

bool GetAlphaTextIsReading(const USW_UIBase* AlphaTextBox)
{
	if (!AlphaTextBox)
	{
		return false;
	}

	if (const USW_AlphaTextBox* NativeAlphaTextBox = Cast<USW_AlphaTextBox>(AlphaTextBox))
	{
		return NativeAlphaTextBox->bIsReading;
	}

	if (const FBoolProperty* Property = FindFProperty<FBoolProperty>(AlphaTextBox->GetClass(), TEXT("bIsReading")))
	{
		return Property->GetPropertyValue_InContainer(AlphaTextBox);
	}

	return false;
}

void SetAlphaTextIsReading(USW_UIBase* AlphaTextBox, const bool bIsReading)
{
	if (!AlphaTextBox)
	{
		return;
	}

	if (USW_AlphaTextBox* NativeAlphaTextBox = Cast<USW_AlphaTextBox>(AlphaTextBox))
	{
		NativeAlphaTextBox->bIsReading = bIsReading;
		return;
	}

	if (const FBoolProperty* Property = FindFProperty<FBoolProperty>(AlphaTextBox->GetClass(), TEXT("bIsReading")))
	{
		Property->SetPropertyValue_InContainer(AlphaTextBox, bIsReading);
	}
}

UWidget* GetAlphaTextThrobber(USW_UIBase* AlphaTextBox)
{
	if (!AlphaTextBox)
	{
		return nullptr;
	}

	if (USW_AlphaTextBox* NativeAlphaTextBox = Cast<USW_AlphaTextBox>(AlphaTextBox))
	{
		if (NativeAlphaTextBox->Throbber)
		{
			return NativeAlphaTextBox->Throbber;
		}

		if (UWidget* NamedThrobber = AlphaTextBox->GetWidgetFromName(TEXT("Throbber")))
		{
			NativeAlphaTextBox->Throbber = NamedThrobber;
			return NamedThrobber;
		}

		if (UUserWidget* UserWidget = Cast<UUserWidget>(AlphaTextBox))
		{
			if (UserWidget->WidgetTree)
			{
				UWidget* FallbackThrobber = UserWidget->WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Throbber_RuntimeFallback"));
				NativeAlphaTextBox->Throbber = FallbackThrobber;
				return FallbackThrobber;
			}
		}

		return nullptr;
	}

	if (const FObjectPropertyBase* Property = FindFProperty<FObjectPropertyBase>(AlphaTextBox->GetClass(), TEXT("Throbber")))
	{
		if (UWidget* ExistingThrobber = Cast<UWidget>(Property->GetObjectPropertyValue_InContainer(AlphaTextBox)))
		{
			return ExistingThrobber;
		}

		if (UWidget* NamedThrobber = AlphaTextBox->GetWidgetFromName(TEXT("Throbber")))
		{
			if (NamedThrobber->IsA(Property->PropertyClass))
			{
				Property->SetObjectPropertyValue_InContainer(AlphaTextBox, NamedThrobber);
			}
			return NamedThrobber;
		}

		if (UUserWidget* UserWidget = Cast<UUserWidget>(AlphaTextBox))
		{
			if (UserWidget->WidgetTree && Property->PropertyClass && Property->PropertyClass->IsChildOf(UWidget::StaticClass()))
			{
				const FName FallbackName = MakeUniqueObjectName(UserWidget->WidgetTree, Property->PropertyClass, TEXT("Throbber_RuntimeFallback"));
				UWidget* FallbackThrobber = UserWidget->WidgetTree->ConstructWidget<UWidget>(Property->PropertyClass, FallbackName);
				Property->SetObjectPropertyValue_InContainer(AlphaTextBox, FallbackThrobber);
				return FallbackThrobber;
			}
		}
	}

	return nullptr;
}

void SetAlphaTextChildrenVisibility(USW_UIBase* AlphaTextBox, const ESlateVisibility Visibility)
{
	if (!AlphaTextBox)
	{
		return;
	}

	if (USW_AlphaTextBox* NativeAlphaTextBox = Cast<USW_AlphaTextBox>(AlphaTextBox))
	{
		for (const TObjectPtr<UWidget>& TextWidget : NativeAlphaTextBox->BP_AlphaTextSingle)
		{
			if (TextWidget)
			{
				TextWidget->SetVisibility(Visibility);
			}
		}
		return;
	}

	const FArrayProperty* ArrayProperty = FindFProperty<FArrayProperty>(AlphaTextBox->GetClass(), TEXT("BP_AlphaTextSingle"));
	if (!ArrayProperty)
	{
		return;
	}

	const FObjectPropertyBase* InnerProperty = CastField<FObjectPropertyBase>(ArrayProperty->Inner);
	if (!InnerProperty)
	{
		return;
	}

	FScriptArrayHelper ArrayHelper(ArrayProperty, ArrayProperty->ContainerPtrToValuePtr<void>(AlphaTextBox));
	for (int32 Index = 0; Index < ArrayHelper.Num(); ++Index)
	{
		if (UWidget* TextWidget = Cast<UWidget>(InnerProperty->GetObjectPropertyValue(ArrayHelper.GetRawPtr(Index))))
		{
			TextWidget->SetVisibility(Visibility);
		}
	}
}

UFunction* FindAlphaTextSetDialogTextFunction(USW_UIBase* AlphaTextBox)
{
	if (!AlphaTextBox)
	{
		return nullptr;
	}

	for (const FName FunctionName : AlphaTextSetDialogTextFunctionNames)
	{
		if (UFunction* Function = AlphaTextBox->FindFunction(FunctionName))
		{
			return Function;
		}
	}

	return nullptr;
}

void SetDialogBoxObjectPropertyValue(UObject* Owner, const FName PropertyName, UObject* Value)
{
	if (!Owner || !Value)
	{
		return;
	}

	const FObjectPropertyBase* ObjectProperty = FindFProperty<FObjectPropertyBase>(Owner->GetClass(), PropertyName);
	if (ObjectProperty && Value->IsA(ObjectProperty->PropertyClass))
	{
		ObjectProperty->SetObjectPropertyValue_InContainer(Owner, Value);
	}
}
}

void USW_DialogBox::NativeConstruct()
{
	if (USW_UIBase* AlphaTextBox = GetAlphaTextBox())
	{
		GetAlphaTextThrobber(AlphaTextBox);
	}

	GetSpeakerNameTextBlock();
	GetSpeakerNameBackground();

	Super::NativeConstruct();

	if (USW_UIBase* AlphaTextBox = GetAlphaTextBox())
	{
		GetAlphaTextThrobber(AlphaTextBox);
	}

	GetSpeakerNameTextBlock();
	GetSpeakerNameBackground();
}

void USW_DialogBox::SetDialogText(const FText& Text)
{
	CurrentDialogText = Text;

	if (USW_UIBase* AlphaTextBox = GetAlphaTextBox())
	{
		SetAlphaTextIsReading(AlphaTextBox, true);
		// Cache the legacy BP variable before invoking its graph; the graph reads it directly.
		if (UWidget* Throbber = GetAlphaTextThrobber(AlphaTextBox))
		{
			Throbber->SetVisibility(ESlateVisibility::Hidden);
		}

		if (UFunction* SetDialogTextFunction = FindAlphaTextSetDialogTextFunction(AlphaTextBox))
		{
			struct FSetDialogTextParams
			{
				FText Text;
			};

			FSetDialogTextParams Params{Text};
			AlphaTextBox->ProcessEvent(SetDialogTextFunction, &Params);
		}
	}

	UpdateDialogText.Broadcast(Text);
}

void USW_DialogBox::SetSpeakerName(const FText& SpeakerName)
{
	CurrentSpeakerName = SpeakerName;

	const FString SpeakerNameString = SpeakerName.ToString().TrimStartAndEnd();
	const bool bHasSpeakerName = !SpeakerNameString.IsEmpty();

	if (UBorder* NameBackground = GetSpeakerNameBackground())
	{
		const FLinearColor BackgroundColor = bHasSpeakerName ? FLinearColor::White : FLinearColor::Transparent;
		NameBackground->SetBrushColor(BackgroundColor);
		NameBackground->SetContentColorAndOpacity(BackgroundColor);
		NameBackground->SetVisibility(bHasSpeakerName ? ESlateVisibility::Visible : ESlateVisibility::HitTestInvisible);
	}

	if (UTextBlock* NameTextBlock = GetSpeakerNameTextBlock())
	{
		NameTextBlock->SetText(bHasSpeakerName ? SpeakerName : FText::GetEmpty());
		NameTextBlock->SetVisibility(bHasSpeakerName ? ESlateVisibility::Visible : ESlateVisibility::HitTestInvisible);
	}
}

bool USW_DialogBox::IsDialogTextRevealing() const
{
	return GetAlphaTextIsReading(GetAlphaTextBox());
}

void USW_DialogBox::CompleteDialogTextReveal()
{
	USW_UIBase* AlphaTextBox = GetAlphaTextBox();
	if (!AlphaTextBox)
	{
		return;
	}

	SetAlphaTextIsReading(AlphaTextBox, false);

	if (UWidget* Throbber = GetAlphaTextThrobber(AlphaTextBox))
	{
		Throbber->SetVisibility(ESlateVisibility::Visible);
	}

	SetAlphaTextChildrenVisibility(AlphaTextBox, ESlateVisibility::Visible);
}

int32 USW_DialogBox::GetCurrentDialogTextLength() const
{
	return CurrentDialogText.ToString().Len();
}

USW_UIBase* USW_DialogBox::GetAlphaTextBox() const
{
	if (USW_UIBase* AlphaTextBox = Cast<USW_UIBase>(GetWidgetFromName(TEXT("BP_AlphaTextBox"))))
	{
		SetDialogBoxObjectPropertyValue(const_cast<USW_DialogBox*>(this), TEXT("BP_AlphaTextBox"), AlphaTextBox);
		return AlphaTextBox;
	}

	return nullptr;
}

UTextBlock* USW_DialogBox::GetSpeakerNameTextBlock() const
{
	if (CachedSpeakerNameTextBlock)
	{
		return CachedSpeakerNameTextBlock;
	}

	for (UObject* OuterObject = GetOuter(); OuterObject; OuterObject = OuterObject->GetOuter())
	{
		if (UUserWidget* OwningWidget = Cast<UUserWidget>(OuterObject))
		{
			if (UTextBlock* NameTextBlock = Cast<UTextBlock>(OwningWidget->GetWidgetFromName(TEXT("TextBlock_Name"))))
			{
				const_cast<USW_DialogBox*>(this)->CachedSpeakerNameTextBlock = NameTextBlock;
				SetDialogBoxObjectPropertyValue(OwningWidget, TEXT("TextBlock_Name"), NameTextBlock);
				return NameTextBlock;
			}
		}
	}

	return nullptr;
}

UBorder* USW_DialogBox::GetSpeakerNameBackground() const
{
	if (CachedSpeakerNameBackground)
	{
		return CachedSpeakerNameBackground;
	}

	for (UObject* OuterObject = GetOuter(); OuterObject; OuterObject = OuterObject->GetOuter())
	{
		if (UUserWidget* OwningWidget = Cast<UUserWidget>(OuterObject))
		{
			if (UBorder* NameBackground = Cast<UBorder>(OwningWidget->GetWidgetFromName(TEXT("NameBoxBackground"))))
			{
				const_cast<USW_DialogBox*>(this)->CachedSpeakerNameBackground = NameBackground;
				SetDialogBoxObjectPropertyValue(OwningWidget, TEXT("NameBoxBackground"), NameBackground);
				return NameBackground;
			}
		}
	}

	return nullptr;
}
