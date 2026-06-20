// Copyright Natali Caggiano. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UnrealClaudeSettings.generated.h"

/**
 * Project-level settings for the UnrealClaude plugin.
 *
 * Edit in: Project Settings -> Plugins -> Unreal Claude.
 * Stored in: Config/DefaultEditor.ini under [/Script/UnrealClaude.UnrealClaudeSettings].
 */
UCLASS(Config=Editor, DefaultConfig, meta=(DisplayName="Unreal Claude"))
class UNREALCLAUDE_API UUnrealClaudeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UUnrealClaudeSettings() = default;

	/**
	 * Use OpenAI Codex CLI instead of Claude CLI for the in-editor assistant panel.
	 *
	 * The UnrealClaude MCP HTTP server still runs inside Unreal Editor. This setting only changes
	 * which local agent CLI the chat panel launches.
	 */
	UPROPERTY(EditAnywhere, Config, Category="Agent", meta=(DisplayName="Use Codex CLI"))
	bool bUseCodexCli = false;

	/** Optional absolute path to codex.exe/codex. Empty means auto-detect from CODEX_CLI_PATH, common install paths, and PATH. */
	UPROPERTY(EditAnywhere, Config, Category="Agent", meta=(EditCondition="bUseCodexCli", DisplayName="Codex CLI Path"))
	FString CodexCliPath;

	/**
	 * Optional path to the Codex JavaScript CLI entrypoint. When set, Codex CLI Path should point
	 * to node.exe/node and this script path is prepended before "exec".
	 */
	UPROPERTY(EditAnywhere, Config, Category="Agent", meta=(EditCondition="bUseCodexCli", DisplayName="Codex CLI Script Path"))
	FString CodexCliScriptPath;

	/**
	 * Optional Codex model provider override. Use "openai" to avoid inheriting a global custom provider
	 * such as DeepSeek from ~/.codex/config.toml. Leave empty to use the user's Codex config as-is.
	 */
	UPROPERTY(EditAnywhere, Config, Category="Agent", meta=(EditCondition="bUseCodexCli", DisplayName="Codex Model Provider Override"))
	FString CodexModelProviderOverride;

	/** Optional Codex model override. Leave empty to use the Codex default for the selected provider. */
	UPROPERTY(EditAnywhere, Config, Category="Agent", meta=(EditCondition="bUseCodexCli", DisplayName="Codex Model Override"))
	FString CodexModelOverride;

	/**
	 * Auto-approve all script execution requests instead of showing the per-call permission dialog.
	 *
	 * When OFF (default), every Python / C++ / Console / Editor Utility script triggered through the
	 * MCP bridge or the in-editor Claude Assistant chat must be approved by the user via a modal dialog.
	 * That is the safest default and matches the existing behaviour.
	 *
	 * Turn ON to opt into a "trusted" workflow where the host (Claude Code session, an MCP-driven agent,
	 * or an automated test harness) drives many scripts in sequence and the per-script confirmation is
	 * the dominant friction. Approvals are skipped silently and an INFO log line is written instead so
	 * the audit trail is preserved.
	 *
	 * WARNING: enabling this means every script the connected agent submits runs immediately without
	 * human confirmation. Only enable on machines and projects where the connected agent is trusted.
	 */
	UPROPERTY(EditAnywhere, Config, Category="Scripts", meta=(DisplayName="Auto-approve script execution"))
	bool bAutoApproveScripts = false;

	virtual FName GetCategoryName() const override { return TEXT("Plugins"); }

#if WITH_EDITOR
	virtual FText GetSectionText() const override { return NSLOCTEXT("UnrealClaude", "SettingsSectionText", "Unreal Claude"); }
#endif
};
