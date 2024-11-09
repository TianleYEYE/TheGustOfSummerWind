// Copyright 2022 - 2024 Dream Moon Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DreamAsyncSetting.generated.h"

/**
 *
 */
UCLASS(Config=DreamAsyncSetting, DefaultConfig)
class DREAMTOOLSASYNC_API UDreamAsyncSetting : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	virtual FName GetCategoryName() const override { return TEXT("Dream Moon"); }
	virtual FName GetSectionName() const override { return TEXT("Dream Async Setting"); }
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="KMeans算法TextureMat", Category="KMeans Texture", Config)
	TSoftObjectPtr<UMaterialInterface> KMeansTextureMat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="KMeans算法Texture插槽名称", Category="KMeans Texture", Config)
	FName KMeansTextureSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="KMeans算法RenderTarget大小", Category="KMeans Texture", Config)
	FIntPoint KMeansTextureSize = FIntPoint(256, 256);
};
