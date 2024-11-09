// - BY DreamMoon 2024 -
// https://dmstudio.top


#include "DreamToolsBlueprintFunctionLibrary.h"


#include "IImageWrapper.h"
#include "ImageUtils.h"
#include "IImageWrapperModule.h"
#include "Kismet/KismetArrayLibrary.h"

void UDreamToolsBlueprintFunctionLibrary::GetViewportScreenshot(FString ImageName, bool SaveUI, EPathMode PathMode)
{
	// 获取Screenshot, 并生成图像到指定位置
	FString SavePath;
	switch (PathMode)
	{
	case EPathMode::EPM_Custom:
		SavePath = ImageName;
		break;
	case EPathMode::EPM_ProjectDir:
		SavePath = FPaths::Combine(FPaths::ProjectDir(), ImageName);
		break;
	case EPathMode::EPM_ProjectSaveDir:
		SavePath = FPaths::Combine(FPaths::ProjectSavedDir(), ImageName);
		break;
	default:
		SavePath = FPaths::Combine(FPaths::ProjectSavedDir(), ImageName);
		break;
	}

	FScreenshotRequest::RequestScreenshot(SavePath, SaveUI, false);
	FDelegateHandle ScreenshotHandle = FScreenshotRequest::OnScreenshotRequestProcessed().AddLambda([ScreenshotHandle]()
	{
		FScreenshotRequest::OnScreenshotRequestProcessed().Remove(ScreenshotHandle);
	});
}

UTexture2D* UDreamToolsBlueprintFunctionLibrary::OpenImage(FString ImageName, EPathMode PathMode)
{
	FString SavePath;
	switch (PathMode)
	{
	case EPathMode::EPM_Custom:
		SavePath = ImageName;
		return FImageUtils::ImportFileAsTexture2D(ImageName);
	case EPathMode::EPM_ProjectDir:
		SavePath = FPaths::ProjectDir();
		break;
	case EPathMode::EPM_ProjectSaveDir:
		SavePath = FPaths::ProjectSavedDir();
		break;
	default:
		SavePath = FPaths::ProjectSavedDir();
		break;
	}
	return FImageUtils::ImportFileAsTexture2D(FPaths::Combine(SavePath, ImageName));
}

TArray<int> UDreamToolsBlueprintFunctionLibrary::sortInt(TArray<int> InArray, bool IsAscending)
{
	TArray<int> Buffer = InArray;
	if (IsAscending)
	{
		Buffer.Sort([](const int& A, const int& B) { return A > B; });
	}
	else
	{
		Buffer.Sort([](const int& A, const int& B) { return A < B; });
	}
	return Buffer;
}

TArray<float> UDreamToolsBlueprintFunctionLibrary::sortFloat(TArray<float> InArray, bool IsAscending)
{
	TArray<float> Buffer = InArray;
	if (IsAscending)
	{
		Buffer.Sort([](const int& A, const int& B) { return A > B; });
	}
	else
	{
		Buffer.Sort([](const int& A, const int& B) { return A < B; });
	}
	return Buffer;
}
