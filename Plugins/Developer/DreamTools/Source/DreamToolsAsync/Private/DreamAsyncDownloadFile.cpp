// - BY DreamMoon 2024 -
// https://dmstudio.top


#include "DreamAsyncDownloadFile.h"

DEFINE_LOG_CATEGORY(LogDreamToolsAsync);

UDreamAsyncDownloadFile* UDreamAsyncDownloadFile::AsyncDownloadFile(UObject* WorldContextObject, const FString& URL, const FString& FilePath)
{
	UDreamAsyncDownloadFile* Node = NewObject<UDreamAsyncDownloadFile>();
	Node->DownloadURL = URL;
	Node->WorldContext = WorldContextObject;
	Node->FilePath = FilePath;
	Node->RegisterWithGameInstance(WorldContextObject);
	return Node;
}

void UDreamAsyncDownloadFile::Activate()
{
	Super::Activate();

	if (DownloadURL.IsEmpty())
	{
		OnDownloadFailed.Broadcast();
		UE_LOG(LogDreamToolsAsync, Error, TEXT("Download URL is empty"));
		SetReadyToDestroy();
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request = FHttpModule::Get().CreateRequest();
	request->SetVerb("GET");
	request->SetURL(DownloadURL);
	request->OnProcessRequestComplete().BindUObject(this, &UDreamAsyncDownloadFile::DownloadRequestHandle); //请求回调
	request->OnRequestProgress().BindUObject(this, &UDreamAsyncDownloadFile::DownloadProgress); // 下载进度
	request->ProcessRequest();
}

void UDreamAsyncDownloadFile::DownloadRequestHandle(FHttpRequestPtr request, FHttpResponsePtr response,
                                                    bool bWasSuccessful)
{
	if (bWasSuccessful && response.IsValid() && EHttpResponseCodes::IsOk(response->GetResponseCode()))
	{
		if (response->GetContentLength() > 0)
		{
			TArray<uint8> EmptyData;
			OnDownloadUpdate.Broadcast(response->GetContentLength(), response->GetContentLength(), EmptyData);
		}

		FString FileSavePath = FilePath;
		FString Path, Filename, Extension;
		FPaths::Split(FileSavePath, Path, Filename, Extension);
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		if (!PlatformFile.DirectoryExists(*Path))
		{
			if (!PlatformFile.CreateDirectoryTree(*Path))
			{
				UE_LOG(LogDreamToolsAsync, Error, TEXT("Create Directory Failed!"));
				OnDownloadFailed.Broadcast();
				SetReadyToDestroy();
				return;
			}
		}

		IFileHandle* FileHandle = PlatformFile.OpenWrite(*FileSavePath);
		if (FileHandle)
		{
			FileHandle->Write(response->GetContent().GetData(), response->GetContentLength());
			delete FileHandle;
			OnDownloadCompleted.Broadcast();
			SetReadyToDestroy();
			return;
		}
		else
		{
			UE_LOG(LogDreamToolsAsync, Error, TEXT("Save File Failed!"));
			OnDownloadFailed.Broadcast();
			SetReadyToDestroy();
			return;
		}
	}
}

void UDreamAsyncDownloadFile::DownloadProgress(FHttpRequestPtr request, int32 bytesSent, int32 bytesReceived)
{
	if (request->GetResponse()->GetContentLength() > 0)
	{
		TArray<uint8> EmptyData;
		OnDownloadUpdate.Broadcast(bytesReceived, request->GetResponse()->GetContentLength(), EmptyData);
	}
}
