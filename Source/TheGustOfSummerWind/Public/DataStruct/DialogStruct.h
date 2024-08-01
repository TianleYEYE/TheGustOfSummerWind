#pragma once

#include "CoreMinimal.h"
#include "DialogStruct.generated.h"

USTRUCT(BlueprintType)
struct THEGUSTOFSUMMERWIND_API FDialogStruct:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText Name=FText();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText Dialog=FText();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UTexture2D>CharacterPortraits_1;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UTexture2D>CharacterPortraits_2;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UTexture2D>CharacterPortraits_3;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UTexture2D>Background;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UTexture2D>GameCG;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<USoundBase>BackgroundSound;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<USoundBase>ConversationalVoice;
	
};
