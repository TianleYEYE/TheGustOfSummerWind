#include "Editor/SW_RefreshBlueprintNodesCommandlet.h"

#if WITH_EDITOR

#include "Engine/Blueprint.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphSchema.h"
#include "FileHelpers.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/PackageName.h"
#include "UObject/Package.h"

USW_RefreshBlueprintNodesCommandlet::USW_RefreshBlueprintNodesCommandlet()
{
	IsClient = false;
	IsEditor = true;
	IsServer = false;
	LogToConsole = true;
}

namespace
{
UEdGraphPin* FindReplacementPin(UEdGraphNode* Node, const UEdGraphPin* OrphanedPin)
{
	if (!Node || !OrphanedPin)
	{
		return nullptr;
	}

	const auto IsUsableReplacement = [OrphanedPin](const UEdGraphPin* Candidate)
	{
		return Candidate
			&& !Candidate->bOrphanedPin
			&& Candidate->Direction == OrphanedPin->Direction
			&& Candidate->PinType.PinCategory == OrphanedPin->PinType.PinCategory;
	};

	static const TArray<FName> PreferredPinNames = {
		TEXT("ReturnValue"),
		TEXT("AsBPLoadScreenViewModel"),
		TEXT("AsBPSystemViewModel"),
		TEXT("DataTable"),
		TEXT("RowDialog")
	};

	for (const FName& PinName : PreferredPinNames)
	{
		if (UEdGraphPin* Candidate = Node->FindPin(PinName, OrphanedPin->Direction))
		{
			if (IsUsableReplacement(Candidate))
			{
				return Candidate;
			}
		}
	}

	UEdGraphPin* OnlyCandidate = nullptr;
	for (UEdGraphPin* Candidate : Node->Pins)
	{
		if (!IsUsableReplacement(Candidate))
		{
			continue;
		}

		if (OnlyCandidate)
		{
			return nullptr;
		}

		OnlyCandidate = Candidate;
	}

	return OnlyCandidate;
}

int32 RepairOrphanedPins(UBlueprint* Blueprint)
{
	if (!Blueprint)
	{
		return 0;
	}

	TArray<UEdGraph*> Graphs;
	Blueprint->GetAllGraphs(Graphs);

	int32 NumRemovedPins = 0;
	for (UEdGraph* Graph : Graphs)
	{
		if (!Graph)
		{
			continue;
		}

		for (UEdGraphNode* Node : Graph->Nodes)
		{
			if (!Node)
			{
				continue;
			}

			for (int32 PinIndex = Node->Pins.Num() - 1; PinIndex >= 0; --PinIndex)
			{
				UEdGraphPin* Pin = Node->Pins[PinIndex];
				if (!Pin || !Pin->bOrphanedPin)
				{
					continue;
				}

				const FName RemovedPinName = Pin->PinName;
				const FString RemovedPinLabel = Pin->PinFriendlyName.IsEmpty()
					? RemovedPinName.ToString()
					: Pin->PinFriendlyName.ToString();
				TArray<UEdGraphPin*> LinkedPins = Pin->LinkedTo;

				if (UEdGraphPin* ReplacementPin = FindReplacementPin(Node, Pin))
				{
					for (UEdGraphPin* LinkedPin : LinkedPins)
					{
						if (LinkedPin
							&& LinkedPin->GetOwningNodeUnchecked()
							&& ReplacementPin->GetOwningNodeUnchecked()
							&& !ReplacementPin->LinkedTo.Contains(LinkedPin))
						{
							if (const UEdGraphSchema* Schema = Graph->GetSchema())
							{
								Schema->TryCreateConnection(ReplacementPin, LinkedPin);
							}
						}
					}
				}

				UE_LOG(LogTemp, Display, TEXT("Removing orphaned pin '%s' from node '%s' in %s"),
					*RemovedPinLabel,
					*Node->GetNodeTitle(ENodeTitleType::ListView).ToString(),
					*Blueprint->GetPathName());
				for (UEdGraphPin* LinkedPin : LinkedPins)
				{
					if (LinkedPin)
					{
						LinkedPin->LinkedTo.Remove(Pin);
					}
				}
				Pin->LinkedTo.Empty();
				Node->RemovePin(Pin);
				++NumRemovedPins;
			}
		}
	}

	return NumRemovedPins;
}

int32 RemoveKnownInvalidNodes(UBlueprint* Blueprint)
{
	if (!Blueprint)
	{
		return 0;
	}

	const FString BlueprintPath = Blueprint->GetPathName();
	int32 NumRemovedNodes = 0;

	if (BlueprintPath == TEXT("/Game/Blueprints/UI/ContinueUI/BP_ContinueUI.BP_ContinueUI"))
	{
		for (int32 GraphIndex = Blueprint->FunctionGraphs.Num() - 1; GraphIndex >= 0; --GraphIndex)
		{
			UEdGraph* FunctionGraph = Blueprint->FunctionGraphs[GraphIndex];
			if (FunctionGraph && FunctionGraph->GetFName() == TEXT("AnimationPlaying"))
			{
				UE_LOG(LogTemp, Display, TEXT("Removing invalid function graph '%s' in %s"), *FunctionGraph->GetName(), *Blueprint->GetPathName());
				FBlueprintEditorUtils::RemoveGraph(Blueprint, FunctionGraph, EGraphRemoveFlags::MarkTransient);
				++NumRemovedNodes;
			}
		}
	}

	TArray<UEdGraph*> Graphs;
	Blueprint->GetAllGraphs(Graphs);

	for (UEdGraph* Graph : Graphs)
	{
		if (!Graph)
		{
			continue;
		}

		for (int32 NodeIndex = Graph->Nodes.Num() - 1; NodeIndex >= 0; --NodeIndex)
		{
			UEdGraphNode* Node = Graph->Nodes[NodeIndex];
			if (!Node)
			{
				continue;
			}

			const FString NodeTitle = Node->GetNodeTitle(ENodeTitleType::ListView).ToString();
			const bool bInvalidTransitionNode =
				BlueprintPath == TEXT("/Game/Blueprints/UI/InGameUI/BP_InGameUI.BP_InGameUI")
				&& NodeTitle.Contains(TEXT("Transition"))
				&& NodeTitle.Contains(TEXT("Get"));
			const bool bRemovedAnimationPlayingNode =
				BlueprintPath == TEXT("/Game/Blueprints/UI/ContinueUI/BP_ContinueUI.BP_ContinueUI")
				&& NodeTitle.Contains(TEXT("Animation Playing"));

			if (bInvalidTransitionNode || bRemovedAnimationPlayingNode)
			{
				UE_LOG(LogTemp, Display, TEXT("Removing invalid node '%s' in %s"), *NodeTitle, *Blueprint->GetPathName());
				Node->BreakAllNodeLinks();
				Node->DestroyNode();
				++NumRemovedNodes;
			}
		}
	}

	return NumRemovedNodes;
}

FString NormalizeBlueprintObjectPath(const FString& RawPath)
{
	FString Path = RawPath;
	Path.TrimStartAndEndInline();
	Path.RemoveFromStart(TEXT("\""));
	Path.RemoveFromEnd(TEXT("\""));
	Path.RemoveFromStart(TEXT("'"));
	Path.RemoveFromEnd(TEXT("'"));

	if (Path.IsEmpty() || Path.Contains(TEXT(".")))
	{
		return Path;
	}

	FString PackagePath;
	FString AssetName;
	if (Path.Split(TEXT("/"), &PackagePath, &AssetName, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
	{
		return FString::Printf(TEXT("%s/%s.%s"), *PackagePath, *AssetName, *AssetName);
	}

	return Path;
}
}

int32 USW_RefreshBlueprintNodesCommandlet::Main(const FString& Params)
{
	TMap<FString, FString> SwitchParams;
	TArray<FString> Tokens;
	TArray<FString> Switches;
	ParseCommandLine(*Params, Tokens, Switches, SwitchParams);

	const FString* AssetListParam = SwitchParams.Find(TEXT("Assets"));
	if (!AssetListParam || AssetListParam->IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Missing -Assets=/Game/Path/BP,/Game/Path/OtherBP"));
		return 1;
	}

	TArray<FString> AssetPaths;
	AssetListParam->ParseIntoArray(AssetPaths, TEXT(","), true);

	TArray<UPackage*> PackagesToSave;
	int32 NumFailed = 0;

	for (const FString& RawPath : AssetPaths)
	{
		const FString ObjectPath = NormalizeBlueprintObjectPath(RawPath);
		UBlueprint* Blueprint = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *ObjectPath));
		if (!Blueprint)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load blueprint: %s"), *ObjectPath);
			++NumFailed;
			continue;
		}

		UE_LOG(LogTemp, Display, TEXT("Refreshing blueprint nodes: %s"), *Blueprint->GetPathName());
		Blueprint->Modify();
		FBlueprintEditorUtils::RefreshAllNodes(Blueprint);
		const int32 NumRemovedPins = RepairOrphanedPins(Blueprint);
		const int32 NumRemovedNodes = RemoveKnownInvalidNodes(Blueprint);
		if (NumRemovedPins > 0 || NumRemovedNodes > 0)
		{
			FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
		}
		FKismetEditorUtilities::CompileBlueprint(Blueprint, EBlueprintCompileOptions::SkipGarbageCollection);

		if (UPackage* Package = Blueprint->GetOutermost())
		{
			Package->SetDirtyFlag(true);
			PackagesToSave.AddUnique(Package);
		}
	}

	if (PackagesToSave.Num() > 0)
	{
		if (!UEditorLoadingAndSavingUtils::SavePackages(PackagesToSave, true))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save one or more refreshed blueprint packages."));
			++NumFailed;
		}
	}

	return NumFailed;
}

#else

USW_RefreshBlueprintNodesCommandlet::USW_RefreshBlueprintNodesCommandlet() = default;

int32 USW_RefreshBlueprintNodesCommandlet::Main(const FString& Params)
{
	return 0;
}

#endif
