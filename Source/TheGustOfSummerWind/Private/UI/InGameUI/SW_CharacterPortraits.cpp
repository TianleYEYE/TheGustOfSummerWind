// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InGameUI/SW_CharacterPortraits.h"

#include "Components/CanvasPanelSlot.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

namespace
{
const FName BodyBlendFactorParamName(TEXT("BodyBlendFactor"));
const FName FaceBlendFactorParamName(TEXT("FaceBlendFactor"));
const FName OpacityParamName(TEXT("Opacity"));
const FName CurrentSpriteParamName(TEXT("CurrentSprite"));
const FName NewSpriteParamName(TEXT("NewSprite"));
const FName CurrentExpressionParamName(TEXT("CurrentExpression"));
const FName NewExpressionParamName(TEXT("NewExpression"));
}

void USW_CharacterPortraits::NativeConstruct()
{
	Super::NativeConstruct();

	if (VerticalPainting && ActiveCharacterId.IsNone())
	{
		VerticalPainting->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (!ParentMaterial || !VerticalPainting)
	{
		return;
	}

	M_FaceChange = UMaterialInstanceDynamic::Create(ParentMaterial, this);
	if (M_FaceChange)
	{
		VerticalPainting->SetBrushFromMaterial(M_FaceChange);
	}
}

void USW_CharacterPortraits::UpdateCharacterEvent(FCharacterSprite CurrentSprite, FCharacterSprite PreviousSprite)
{
	ApplyCharacterSprite(CurrentSprite, PreviousSprite);
}

void USW_CharacterPortraits::UpateSpriteAndExpression(FCharacterSprite CurrentSprite, FCharacterSprite PreviousSprite)
{
	if (!M_FaceChange)
	{
		return;
	}

	// Move the current "New" textures into "Current" positions (the previous blend target becomes the current display)
	UTexture* OldNewSprite = nullptr;
	if (M_FaceChange->GetTextureParameterValue(NewSpriteParamName, OldNewSprite) && OldNewSprite)
	{
		M_FaceChange->SetTextureParameterValue(CurrentSpriteParamName, OldNewSprite);
	}

	// Set the new incoming sprite as the new blend target
	if (CurrentSprite.CurrentSprite)
	{
		M_FaceChange->SetTextureParameterValue(NewSpriteParamName, CurrentSprite.CurrentSprite);
	}

	// Reset body blend factor and start lerp
	M_FaceChange->SetScalarParameterValue(BodyBlendFactorParamName, 0.0f);
	BodySwitch();

	// Move the current "New" expression into "Current" position
	UTexture* OldNewExpression = nullptr;
	if (M_FaceChange->GetTextureParameterValue(NewExpressionParamName, OldNewExpression) && OldNewExpression)
	{
		M_FaceChange->SetTextureParameterValue(CurrentExpressionParamName, OldNewExpression);
	}

	// Set the new incoming face as the new blend target
	if (CurrentSprite.Face)
	{
		M_FaceChange->SetTextureParameterValue(NewExpressionParamName, CurrentSprite.Face);
	}

	// Reset face blend factor and start lerp
	M_FaceChange->SetScalarParameterValue(FaceBlendFactorParamName, 0.0f);
	FaceSwitch();

	// Start opacity appear animation
	SpriteAppear();
}

void USW_CharacterPortraits::ApplyCharacterSprite(const FCharacterSprite& CurrentSprite, const FCharacterSprite& PreviousSprite)
{
	if (CurrentSprite.bHide)
	{
		HideCharacter(CurrentSprite.Transition);
		return;
	}

	if (!CurrentSprite.CharacterId.IsNone())
	{
		ActiveCharacterId = CurrentSprite.CharacterId;
	}

	if (CurrentSprite.Position != ECharacterSlotPosition::Inherit)
	{
		SetCharacterPosition(CurrentSprite.Position);
	}

	SetCharacterMirror(CurrentSprite.bMirror);
	SetCharacterZOrder(CurrentSprite.ZOrder);

	const bool bHasSpriteUpdate = CurrentSprite.CurrentSprite || CurrentSprite.Face;
	if (bHasSpriteUpdate || !CurrentSprite.bKeepPrevious)
	{
		UpateSpriteAndExpression(CurrentSprite, PreviousSprite);
	}

	ShowCharacter(CurrentSprite.Transition);
}

void USW_CharacterPortraits::SetCharacterPosition(const ECharacterSlotPosition Position)
{
	if (!VerticalPainting)
	{
		return;
	}

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(VerticalPainting->Slot);
	if (!CanvasSlot)
	{
		return;
	}

	switch (Position)
	{
	case ECharacterSlotPosition::Left:
		CanvasSlot->SetAnchors(FAnchors(0.25f, 1.0f));
		CanvasSlot->SetAlignment(FVector2D(0.5f, 1.0f));
		break;
	case ECharacterSlotPosition::Center:
		CanvasSlot->SetAnchors(FAnchors(0.5f, 1.0f));
		CanvasSlot->SetAlignment(FVector2D(0.5f, 1.0f));
		break;
	case ECharacterSlotPosition::Right:
		CanvasSlot->SetAnchors(FAnchors(0.75f, 1.0f));
		CanvasSlot->SetAlignment(FVector2D(0.5f, 1.0f));
		break;
	case ECharacterSlotPosition::TrueCenter:
		CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		break;
	default:
		return;
	}

	CanvasSlot->SetOffsets(FMargin(0.0f));
}

void USW_CharacterPortraits::SetCharacterMirror(const bool bMirror)
{
	if (VerticalPainting)
	{
		VerticalPainting->SetRenderScale(FVector2D(bMirror ? -1.0f : 1.0f, 1.0f));
	}
}

void USW_CharacterPortraits::SetCharacterZOrder(const int32 ZOrder)
{
	if (!VerticalPainting)
	{
		return;
	}

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(VerticalPainting->Slot))
	{
		CanvasSlot->SetZOrder(ZOrder);
	}
}

void USW_CharacterPortraits::ShowCharacter(const ECharacterTransitionType Transition)
{
	if (VerticalPainting)
	{
		VerticalPainting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (M_FaceChange && Transition == ECharacterTransitionType::Fade)
	{
		M_FaceChange->SetScalarParameterValue(OpacityParamName, 0.0f);
		SpriteAppear();
	}
}

void USW_CharacterPortraits::HideCharacter(const ECharacterTransitionType Transition)
{
	ActiveCharacterId = NAME_None;

	if (VerticalPainting)
	{
		VerticalPainting->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool USW_CharacterPortraits::IsCharacterVisible() const
{
	return VerticalPainting && VerticalPainting->GetVisibility() != ESlateVisibility::Collapsed;
}

void USW_CharacterPortraits::BodySwitch()
{
	if (!M_FaceChange)
	{
		return;
	}

	float CurrentBlend = 0.0f;
	M_FaceChange->GetScalarParameterValue(BodyBlendFactorParamName, CurrentBlend);

	if (CurrentBlend < 1.0f)
	{
		const float NextBlend = FMath::Min(CurrentBlend + BlendStepSize, 1.0f);
		M_FaceChange->SetScalarParameterValue(BodyBlendFactorParamName, NextBlend);

		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(
			UnusedHandle,
			FTimerDelegate::CreateUObject(this, &USW_CharacterPortraits::BodySwitch),
			BlendTickInterval,
			false
		);
	}
}

void USW_CharacterPortraits::FaceSwitch()
{
	if (!M_FaceChange)
	{
		return;
	}

	float CurrentBlend = 0.0f;
	M_FaceChange->GetScalarParameterValue(FaceBlendFactorParamName, CurrentBlend);

	if (CurrentBlend < 1.0f)
	{
		const float NextBlend = FMath::Min(CurrentBlend + BlendStepSize, 1.0f);
		M_FaceChange->SetScalarParameterValue(FaceBlendFactorParamName, NextBlend);

		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(
			UnusedHandle,
			FTimerDelegate::CreateUObject(this, &USW_CharacterPortraits::FaceSwitch),
			BlendTickInterval,
			false
		);
	}
}

void USW_CharacterPortraits::SpriteAppear()
{
	if (!M_FaceChange)
	{
		return;
	}

	float CurrentOpacity = 0.0f;
	M_FaceChange->GetScalarParameterValue(OpacityParamName, CurrentOpacity);

	if (CurrentOpacity < 1.0f)
	{
		const float NextOpacity = FMath::Min(CurrentOpacity + BlendStepSize, 1.0f);
		M_FaceChange->SetScalarParameterValue(OpacityParamName, NextOpacity);

		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(
			UnusedHandle,
			FTimerDelegate::CreateUObject(this, &USW_CharacterPortraits::SpriteAppear),
			BlendTickInterval,
			false
		);
	}
}
