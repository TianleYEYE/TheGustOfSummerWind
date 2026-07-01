// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "DataStruct/DialogStruct.h"
#include "UI/SW_UIBase.h"
#include "SW_CharacterPortraits.generated.h"

UCLASS()
class THEGUSTOFSUMMERWIND_API USW_CharacterPortraits : public USW_UIBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> VerticalPainting;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	TObjectPtr<UMaterialInterface> ParentMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float BlendStepSize = 0.02f;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float BlendTickInterval = 0.01f;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Character")
	FName ActiveCharacterId;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void UpdateCharacterEvent(FCharacterSprite CurrentSprite, FCharacterSprite PreviousSprite);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void UpateSpriteAndExpression(FCharacterSprite CurrentSprite, FCharacterSprite PreviousSprite);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void ApplyCharacterSprite(const FCharacterSprite& CurrentSprite, const FCharacterSprite& PreviousSprite);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetCharacterPosition(ECharacterSlotPosition Position);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetCharacterMirror(bool bMirror);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetCharacterZOrder(int32 ZOrder);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void ShowCharacter(ECharacterTransitionType Transition = ECharacterTransitionType::None);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void HideCharacter(ECharacterTransitionType Transition = ECharacterTransitionType::None);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	bool IsCharacterVisible() const;

protected:
	void BodySwitch();
	void FaceSwitch();
	void SpriteAppear();

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> M_FaceChange;
};
