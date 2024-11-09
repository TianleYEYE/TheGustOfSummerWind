// - BY DreamMoon 2024 -
// https://dmstudio.top

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DreamToolsCommon.h"
#include "UObject/UnrealTypePrivate.h"
#include "DreamToolsBlueprintFunctionLibrary.generated.h"

/**
 * 
 */



UCLASS(DisplayName="DreamTools蓝图函数库")
class DREAMTOOLS_API UDreamToolsBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Dream工具箱|View", DisplayName="获取视口截图")
	static void GetViewportScreenshot(FString ImageName, bool SaveUI, EPathMode PathMode = EPathMode::EPM_ProjectSaveDir);

    UFUNCTION(BlueprintCallable, Category="Dream工具箱|File", DisplayName="打开图像")
	static UTexture2D* OpenImage(FString ImageName, EPathMode PathMode = EPathMode::EPM_ProjectSaveDir);

	UFUNCTION(BlueprintCallable, Category="Dream工具箱|Sort", DisplayName="对整形进行排序")
	static TArray<int> sortInt(TArray<int> InArray, bool IsAscending);

	UFUNCTION(BlueprintCallable, Category="Dream工具箱|Sort", DisplayName="对浮点型进行排序")
	static TArray<float> sortFloat(TArray<float> InArray, bool IsAscending);
};
