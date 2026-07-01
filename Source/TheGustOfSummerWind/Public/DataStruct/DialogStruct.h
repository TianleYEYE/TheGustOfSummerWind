#pragma once

#include "CoreMinimal.h"
#include "DialogStruct.generated.h"

UENUM(BlueprintType)
enum class ECharacterSlotPosition : uint8
{
	Inherit,
	Left,
	Center,
	Right,
	TrueCenter
};

UENUM(BlueprintType)
enum class ECharacterTransitionType : uint8
{
	Inherit,
	None,
	Fade
};

USTRUCT(BlueprintType)
struct THEGUSTOFSUMMERWIND_API FCharacterSprite:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName CharacterId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D>CurrentSprite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D>Face;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharacterSlotPosition Position = ECharacterSlotPosition::Inherit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharacterTransitionType Transition = ECharacterTransitionType::Inherit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bKeepPrevious = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bHide = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bMirror = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ZOrder = 0;
};

USTRUCT(BlueprintType)
struct THEGUSTOFSUMMERWIND_API FDialogStruct:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText Name=FText();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText Dialog=FText();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FCharacterSprite Character_1;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FCharacterSprite Character_2;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FCharacterSprite Character_3;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UTexture2D>Background;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<USoundBase>BackgroundSound;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<USoundBase>ConversationalVoice;

	// UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	// bool bIsStarted = false ;
};

