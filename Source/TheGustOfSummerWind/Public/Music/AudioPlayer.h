#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioPlayer.generated.h"

UCLASS()
class THEGUSTOFSUMMERWIND_API AAudioPlayer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAudioPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Audio")
	TObjectPtr<UAudioComponent>audioPlayer;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Audio")
	TObjectPtr<UAudioComponent>ConversationalVoicePlayer;

	UPROPERTY( BlueprintReadOnly, Category = "MySceneComponent")
	TObjectPtr<USceneComponent>SceneComponent;
	
	void MenuMusicPlay();


	
	bool menuMusicIsPlay=false;
	
};
