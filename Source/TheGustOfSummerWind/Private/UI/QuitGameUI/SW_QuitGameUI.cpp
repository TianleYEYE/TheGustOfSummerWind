// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuitGameUI/SW_QuitGameUI.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Music/AudioPlayer.h"

void USW_QuitGameUI::NativeConstruct()
{
	Super::NativeConstruct();

	auto TempActor = UGameplayStatics::GetActorOfClass(GetWorld(),AudioPlayer->StaticClass());
	AudioPlayer = Cast<AAudioPlayer>(TempActor);
	
	BP_Yes->Yes.AddUObject(this,&USW_QuitGameUI::QuitGame);
	BP_No->No.AddUObject(this, &USW_QuitGameUI::ReturnGame);
	
}

void USW_QuitGameUI::QuitGame()
{
	PlayAnimation(Fade);
	AudioPlayer->GetWorldTimerManager().SetTimer(PlayFade, this, &USW_QuitGameUI::QuitGameImplement, 0.5f, false);
}

void USW_QuitGameUI::QuitGameImplement()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void USW_QuitGameUI::ReturnGame()
{
	PlayAnimation(Fade);
	AudioPlayer->GetWorldTimerManager().SetTimer(PlayFade, this, &USW_QuitGameUI::ReturnGameImlement, 0.5f, false);
}

void USW_QuitGameUI::ReturnGameImlement()
{
	RemoveFromParent();
}

