#pragma once

#include "CoreMinimal.h"
#include "BacklogEntry.generated.h"

USTRUCT(BlueprintType)
struct THEGUSTOFSUMMERWIND_API FSW_BacklogEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Backlog")
	FText SpeakerName;

	UPROPERTY(BlueprintReadOnly, Category = "Backlog")
	FText Dialogue;

	UPROPERTY(BlueprintReadOnly, Category = "Backlog")
	FName DialogKey = NAME_None;

	UPROPERTY(BlueprintReadOnly, Category = "Backlog")
	int32 ChapterIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly, Category = "Backlog")
	int32 RowIndex = INDEX_NONE;
};
