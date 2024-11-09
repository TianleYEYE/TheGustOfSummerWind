// Copyright 2022 - 2024 Dream Moon Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "DreamAsyncCommon.h"
#include "DreamAsyncTextureKMeans.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleted, const TArray<FLinearColor>&, Colors);

UCLASS()
class DREAMTOOLSASYNC_API UDreamAsyncTextureKMeans : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/*
	 *	Async Texture2D KMeans algorithm
	 *	@param Texture2D				聚类纹理
	 *	@param K						聚类数量
	 *	@param MaxIterations			最大迭代次数
	 *	@param Centroids				初始聚类中心
	 */
	UFUNCTION(BlueprintCallable, DisplayName="TextureKMeans算法", Category="Dream工具箱|Async",
		meta=(BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm =
			"WorldContextObject"))
	static UDreamAsyncTextureKMeans* AsyncColorKMeans(UTexture2D* Texture2D, int32 K, int32 MaxIterations, UObject* WorldContextObject);

	UPROPERTY(BlueprintAssignable)
	FOnCompleted OnCompleted;

private:

	TArray<FLinearColor> NodeColors;
	int32 NodeK;
	int32 NodeMaxIterations;

	UPROPERTY()
	UObject* WorldContext;
	UPROPERTY()
	UTexture2D* Texture;

	// 计算线性颜色之间的距离平方
	float LinearColorDistanceSquared(const FLinearColor& A, const FLinearColor& B);
	// 分配线性颜色到最近的聚类中心
	void AssignLinearColorToClusters(const TArray<FLinearColor>& Colors, TArray<FDreamColorKMeansCluster>& Clusters, TArray<int32>& ClusterIndices);
	// 更新聚类中心
	void UpdateLinearClusterCentroids(TArray<FDreamColorKMeansCluster>& Clusters);

	// 激活Node
	virtual void Activate() override;
};
