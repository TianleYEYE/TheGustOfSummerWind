// Copyright 2022 - 2024 Dream Moon Team. All Rights Reserved.


#include "DreamAsyncColorKMeans.h"


UDreamAsyncColorKMeans* UDreamAsyncColorKMeans::AsyncColorKMeans(TArray<FLinearColor> Colors, int32 K, int32 MaxIterations, UObject* WorldContextObject)
{
	UDreamAsyncColorKMeans* Node = NewObject<UDreamAsyncColorKMeans>();
	Node->_Colors = Colors;
	Node->_K = K;
	Node->_MaxIterations = MaxIterations;
	Node->RegisterWithGameInstance(WorldContextObject);
	return Node;
}

float UDreamAsyncColorKMeans::LinearColorDistanceSquared(const FLinearColor& A, const FLinearColor& B)
{
	return FMath::Square(A.R - B.R) + FMath::Square(A.G - B.G) + FMath::Square(A.B - B.B) + FMath::Square(A.A - B.A);
}

void UDreamAsyncColorKMeans::AssignLinearColorToClusters(const TArray<FLinearColor>& Colors,
TArray<FDreamColorKMeansCluster>& Clusters, TArray<int32>& ClusterIndices)
{
	for (int32 i = 0; i < Colors.Num(); ++i)
	{
		float MinDistance = TNumericLimits<float>::Max();
		int32 ClosestClusterIndex = -1;

		for (int32 j = 0; j < Clusters.Num(); ++j)
		{
			float Distance = LinearColorDistanceSquared(Colors[i], Clusters[j].Centroid);
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestClusterIndex = j;
			}
		}

		ClusterIndices[i] = ClosestClusterIndex; // 记录每个线性颜色点所属的聚类索引
		Clusters[ClosestClusterIndex].Points.Add(Colors[i]); // 将线性颜色点添加到最近的聚类中心的点集合中
	}
}

void UDreamAsyncColorKMeans::UpdateLinearClusterCentroids(TArray<FDreamColorKMeansCluster>& Clusters)
{
	for (int32 i = 0; i < Clusters.Num(); ++i)
	{
		if (Clusters[i].Points.Num() > 0)
		{
			FLinearColor NewCentroid(0, 0, 0, 0);

			for (const FLinearColor& Point : Clusters[i].Points)
			{
				NewCentroid.R += Point.R;
				NewCentroid.G += Point.G;
				NewCentroid.B += Point.B;
				NewCentroid.A += Point.A;
			}

			NewCentroid.R /= Clusters[i].Points.Num();
			NewCentroid.G /= Clusters[i].Points.Num();
			NewCentroid.B /= Clusters[i].Points.Num();
			NewCentroid.A /= Clusters[i].Points.Num();

			Clusters[i].Centroid = NewCentroid;
			Clusters[i].Points.Empty(); // 清空当前聚类的线性颜色点集合
		}
	}
}


void UDreamAsyncColorKMeans::Activate()
{
	// KMeans
	check(_K > 0 && _K <= _Colors.Num());
	TArray<FDreamColorKMeansCluster> Clusters;
	for (int32 i = 0; i < _K; ++i)
	{
		FDreamColorKMeansCluster Cluster;
		Cluster.Centroid = _Colors[i]; // 简单起见，假设前K个线性颜色作为初始聚类中心
		Clusters.Add(Cluster);
	}

	TArray<int> ClusterIndices;
	ClusterIndices.Init(-1, _Colors.Num());

	bool bConverged = false;
	int32 Iteration = 0;

	while (!bConverged && Iteration < _MaxIterations)
	{
		TArray<FDreamColorKMeansCluster> OldClusters = Clusters;

		// 分配线性颜色到最近的聚类中心
		AssignLinearColorToClusters(_Colors, Clusters, ClusterIndices);

		// 更新聚类中心位置
		UpdateLinearClusterCentroids(Clusters);

		// 检查聚类中心是否收敛
		bConverged = true;
		for (int32 i = 0; i < Clusters.Num(); ++i)
		{
			if (LinearColorDistanceSquared(Clusters[i].Centroid, OldClusters[i].Centroid) > KINDA_SMALL_NUMBER)
			{
				bConverged = false;
				break;
			}
		}

		Iteration++;
	}

	// 提取每个聚类的中心线性颜色作为结果
	TArray<FLinearColor> ResultColors;
	for (const FDreamColorKMeansCluster& Cluster : Clusters)
	{
		ResultColors.Add(Cluster.Centroid);
	}

	OnCompletedK.Broadcast(ResultColors);
	SetReadyToDestroy();
}
