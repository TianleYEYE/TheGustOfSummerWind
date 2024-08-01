#include "Music/AudioPlayer.h"
#include "Components/AudioComponent.h"

// Sets default values
AAudioPlayer::AAudioPlayer()
{
	audioPlayer=CreateDefaultSubobject<UAudioComponent>(TEXT("CustomAudio1"));
	SceneComponent=CreateDefaultSubobject<USceneComponent>(TEXT("CustomScene"));
	ConversationalVoicePlayer=CreateDefaultSubobject<UAudioComponent>(TEXT("CustomAudio2"));

	RootComponent=SceneComponent;
	audioPlayer->SetupAttachment(SceneComponent);
	ConversationalVoicePlayer->SetupAttachment(SceneComponent);
}

void AAudioPlayer::MenuMusicPlay()
{
	USoundBase *menuSound=LoadObject<USoundBase>(this,TEXT("/Game/Assets/Audio/01_Irreplaceable_memories_-Main_theme-_Cue.01_Irreplaceable_memories_-Main_theme-_Cue"));
	audioPlayer->SetSound(menuSound);
	audioPlayer->Play();
	menuMusicIsPlay =true;
}

void AAudioPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	MenuMusicPlay();
}

void AAudioPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

