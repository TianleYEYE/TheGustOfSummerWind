// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InGameUI/SW_CharacterPortraits.h"

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
	UpateSpriteAndExpression(CurrentSprite, PreviousSprite);
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
