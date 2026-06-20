// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/SW_DialogBox.h"

#include "Components/Widget.h"
#include "UObject/UnrealType.h"

namespace
{
UObject* GetObjectPropertyValue(UObject* Owner, const FName PropertyName)
{
	if (!Owner)
	{
		return nullptr;
	}

	const FObjectPropertyBase* ObjectProperty = FindFProperty<FObjectPropertyBase>(Owner->GetClass(), PropertyName);
	return ObjectProperty ? ObjectProperty->GetObjectPropertyValue_InContainer(Owner) : nullptr;
}

bool GetBoolPropertyValue(const UObject* Owner, const FName PropertyName)
{
	if (!Owner)
	{
		return false;
	}

	const FBoolProperty* BoolProperty = FindFProperty<FBoolProperty>(Owner->GetClass(), PropertyName);
	return BoolProperty ? BoolProperty->GetPropertyValue_InContainer(Owner) : false;
}

void SetBoolPropertyValue(UObject* Owner, const FName PropertyName, const bool bValue)
{
	if (!Owner)
	{
		return;
	}

	if (const FBoolProperty* BoolProperty = FindFProperty<FBoolProperty>(Owner->GetClass(), PropertyName))
	{
		BoolProperty->SetPropertyValue_InContainer(Owner, bValue);
	}
}
}

void USW_DialogBox::NativeConstruct()
{
	Super::NativeConstruct();
}

void USW_DialogBox::SetDialogText(FText& Text)
{
	CurrentDialogText = Text;
	UpdateDialogText.Broadcast(Text);
}

bool USW_DialogBox::IsDialogTextRevealing() const
{
	const UObject* AlphaTextBox = GetObjectPropertyValue(const_cast<USW_DialogBox*>(this), TEXT("BP_AlphaTextBox"));
	return GetBoolPropertyValue(AlphaTextBox, TEXT("bIsReading"));
}

void USW_DialogBox::CompleteDialogTextReveal()
{
	UObject* AlphaTextBox = GetObjectPropertyValue(this, TEXT("BP_AlphaTextBox"));
	if (!AlphaTextBox)
	{
		return;
	}

	SetBoolPropertyValue(AlphaTextBox, TEXT("bIsReading"), false);

	if (UWidget* Throbber = Cast<UWidget>(GetObjectPropertyValue(AlphaTextBox, TEXT("Throbber"))))
	{
		Throbber->SetVisibility(ESlateVisibility::Visible);
	}

	const FArrayProperty* TextWidgetsProperty = FindFProperty<FArrayProperty>(AlphaTextBox->GetClass(), TEXT("BP_AlphaTextSingle"));
	if (!TextWidgetsProperty)
	{
		return;
	}

	const FObjectPropertyBase* InnerObjectProperty = CastField<FObjectPropertyBase>(TextWidgetsProperty->Inner);
	if (!InnerObjectProperty)
	{
		return;
	}

	FScriptArrayHelper TextWidgets(TextWidgetsProperty, TextWidgetsProperty->ContainerPtrToValuePtr<void>(AlphaTextBox));
	for (int32 Index = 0; Index < TextWidgets.Num(); ++Index)
	{
		UObject* TextWidgetObject = InnerObjectProperty->GetObjectPropertyValue(TextWidgets.GetRawPtr(Index));
		if (UWidget* TextWidget = Cast<UWidget>(TextWidgetObject))
		{
			TextWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

int32 USW_DialogBox::GetCurrentDialogTextLength() const
{
	return CurrentDialogText.ToString().Len();
}
