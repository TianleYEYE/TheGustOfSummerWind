#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "SW_RefreshBlueprintNodesCommandlet.generated.h"

UCLASS()
class THEGUSTOFSUMMERWIND_API USW_RefreshBlueprintNodesCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	USW_RefreshBlueprintNodesCommandlet();

	virtual int32 Main(const FString& Params) override;
};
