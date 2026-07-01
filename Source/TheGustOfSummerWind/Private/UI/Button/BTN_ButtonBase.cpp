// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonBase.h"

#include "Animation/WidgetAnimation.h"

DEFINE_LOG_CATEGORY_STATIC(LogSWButtonAnimation, Log, All);

void UBTN_ButtonBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (UWidgetAnimation* PrimaryActionAnimation = GetPrimaryActionAnimation())
	{
		UE_LOG(LogSWButtonAnimation, Verbose, TEXT("[%s] Bound primary action animation: %s"),
			*GetName(),
			*GetNameSafe(PrimaryActionAnimation));

		PrimaryActionAnimationFinishedEvent.Unbind();
		PrimaryActionAnimationFinishedEvent.BindDynamic(this, &UBTN_ButtonBase::OnPrimaryActionAnimationFinished);
		BindToAnimationFinished(PrimaryActionAnimation, PrimaryActionAnimationFinishedEvent);
	}
	else
	{
		UE_LOG(LogSWButtonAnimation, Verbose, TEXT("[%s] No primary action animation bound; action will trigger immediately."), *GetName());
	}
}

void UBTN_ButtonBase::DelegatedAgentAndPlayAnimation()
{
}

UWidgetAnimation* UBTN_ButtonBase::GetPrimaryActionAnimation() const
{
	return nullptr;
}

void UBTN_ButtonBase::BroadcastPrimaryAction()
{
}

void UBTN_ButtonBase::PlayPrimaryActionAnimationOrBroadcast()
{
	if (bPendingActionBroadcast)
	{
		UE_LOG(LogSWButtonAnimation, Warning, TEXT("[%s] Ignored primary action trigger because an action animation is already pending."), *GetName());
		return;
	}

	if (UWidgetAnimation* PrimaryActionAnimation = GetPrimaryActionAnimation())
	{
		UE_LOG(LogSWButtonAnimation, Log, TEXT("[%s] Playing primary action animation: %s"),
			*GetName(),
			*GetNameSafe(PrimaryActionAnimation));

		bPendingActionBroadcast = true;
		StopAnimation(PrimaryActionAnimation);
		PlayAnimation(PrimaryActionAnimation);
		return;
	}

	UE_LOG(LogSWButtonAnimation, Log, TEXT("[%s] No primary action animation; broadcasting action immediately."), *GetName());
	BroadcastPrimaryAction();
}

void UBTN_ButtonBase::OnPrimaryActionAnimationFinished()
{
	if (!bPendingActionBroadcast)
	{
		UE_LOG(LogSWButtonAnimation, Verbose, TEXT("[%s] Received primary action animation finished callback without a pending action."), *GetName());
		return;
	}

	UE_LOG(LogSWButtonAnimation, Log, TEXT("[%s] Primary action animation finished; broadcasting action."), *GetName());
	bPendingActionBroadcast = false;
	BroadcastPrimaryAction();
}
