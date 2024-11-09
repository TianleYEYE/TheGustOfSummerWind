#pragma once

#include "CoreMinimal.h"
#include "DreamAsyncCommon.generated.h"

USTRUCT(BlueprintType)
struct FDreamColorKMeansCluster
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Centroid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLinearColor> Points;
};