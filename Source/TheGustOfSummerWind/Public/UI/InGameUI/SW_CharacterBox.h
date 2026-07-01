#pragma once

#include "CoreMinimal.h"
#include "DataStruct/DialogStruct.h"
#include "UI/SW_UIBase.h"
#include "SW_CharacterBox.generated.h"

class USW_CharacterPortraits;
class UUserWidget;

UCLASS()
class THEGUSTOFSUMMERWIND_API USW_CharacterBox : public USW_UIBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<USW_CharacterPortraits> BP_CharacterPortraits_0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<USW_CharacterPortraits> BP_CharacterPortraits_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<USW_CharacterPortraits> BP_CharacterPortraits_2;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void InitializeFromOwnerWidget(UUserWidget* OwnerWidget);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void UpdateCharacters(const FDialogStruct& CurrentDialog, const FDialogStruct& PreviousDialog);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	USW_CharacterPortraits* GetCharacterPortraitAt(int32 Index) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	USW_CharacterPortraits* GetCharacterPortraitById(FName CharacterId) const;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void ClearCharacterBindings();

private:
	void CachePortraitReferences(UUserWidget* LegacyOwnerWidget);
	USW_CharacterPortraits* ResolvePortraitForSprite(const FCharacterSprite& Sprite, int32 PreferredIndex);
	USW_CharacterPortraits* FindAvailablePortrait(int32 PreferredIndex) const;
	void ApplySpriteToPortrait(const FCharacterSprite& CurrentSprite, const FCharacterSprite& PreviousSprite, int32 PreferredIndex);
	void RemoveBindingsForPortrait(USW_CharacterPortraits* Portrait);

	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<USW_CharacterPortraits>> ActivePortraitsByCharacterId;
};
