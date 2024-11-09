// Copyright 2022 - 2024 Dream Moon Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "DreamAsyncCommon.h"
#include "DreamAsyncColorKMeans.generated.h"

/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompletedK, const TArray<FLinearColor>&, Colors);

UCLASS()
class DREAMTOOLSASYNC_API UDreamAsyncColorKMeans : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, DisplayName="颜色KMeans算法", Category="Dream工具箱|Async", meta=(BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "WorldContextObject"))
	static UDreamAsyncColorKMeans* AsyncColorKMeans(TArray<FLinearColor> Colors, int32 K, int32 MaxIterations, UObject* WorldContextObject);

	UPROPERTY(BlueprintAssignable, DisplayName="Completed")
	FOnCompletedK OnCompletedK;

private:
	TArray<FLinearColor>_Colors;
	int32 _K;
	int32 _MaxIterations;

	float LinearColorDistanceSquared(const FLinearColor& A, const FLinearColor& B);
	void AssignLinearColorToClusters(const TArray<FLinearColor>& Colors, TArray<FDreamColorKMeansCluster>& Clusters, TArray<int32>& ClusterIndices);
	void UpdateLinearClusterCentroids(TArray<FDreamColorKMeansCluster>& Clusters);

	virtual void Activate() override;
};
