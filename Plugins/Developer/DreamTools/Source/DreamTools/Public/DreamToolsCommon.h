#pragma once

#include "CoreMinimal.h"
#include "DreamToolsCommon.generated.h"

UENUM(BlueprintType, DisplayName = "E路径模式")
enum class EPathMode : uint8
{
	EPM_Custom				= 0 UMETA(DisplayName = "自定义路径"),
	EPM_ProjectDir			= 1 UMETA(DisplayName = "项目路径"),
	EPM_ProjectSaveDir		= 2 UMETA(DisplayName = "项目保存路径")
};

USTRUCT(BlueprintType)
struct FDreamToolsFileInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FileName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FilePath;
};