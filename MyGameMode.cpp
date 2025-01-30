// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"
#include "TimePlatform.h"

// Sets default values
AMyGameMode::AMyGameMode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	SpawnPlatforms();
}

void AMyGameMode::SpawnPlatforms()
{
	if (!PlatformClass) return;

	for (int i = 0; i < 10; i++)
	{
		FVector SpawnLocation = FVector(
			FMath::RandRange(-7500.0f, -5000.0f), // X위치 랜덤
			FMath::RandRange(4000.0f, 5000.0f), // Y위치 랜덤
			3650	// Z위치 고정
		);
		ATimePlatform* SpawnedPlatform =
			GetWorld()->SpawnActor<ATimePlatform>(PlatformClass, SpawnLocation, FRotator(0, 0, 0));
		if (SpawnedPlatform)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawned Platform at!!!: %s"), *SpawnLocation.ToString());
		}
	}
}




