// - BY DreamMoon 2024 -
// https://dmstudio.top

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "DreamToolsAsync.h"
#include "Http.h"
#include "DreamAsyncDownloadFile.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDownloadUpdate,
	const int32&, ReceivedDataInByte,
	const int32&, TotalDataInByte,
	const TArray<uint8>&, BinaryData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDownloadAction);

UCLASS()
class DREAMTOOLSASYNC_API UDreamAsyncDownloadFile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, DisplayName="异步下载文件", Category="Dream工具箱|Async|NET", meta=(BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "WorldContextObject"))
	static UDreamAsyncDownloadFile* AsyncDownloadFile(UObject* WorldContextObject, const FString& URL, const FString& FilePath);

	UPROPERTY(BlueprintAssignable, DisplayName="下载状态更新")
	FOnDownloadUpdate OnDownloadUpdate;

	UPROPERTY(BlueprintAssignable, DisplayName="下载完成")
	FDownloadAction OnDownloadCompleted;

	UPROPERTY(BlueprintAssignable, DisplayName="下载失败")
	FDownloadAction OnDownloadFailed;

private:
	virtual void Activate() override;

	UPROPERTY()
	UObject* WorldContext;
	FString FilePath;
	FString DownloadURL;
	
	void DownloadRequestHandle(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);
	void DownloadProgress(FHttpRequestPtr request, int32 bytesSent, int32 bytesReceived);
	
};
