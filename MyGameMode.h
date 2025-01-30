// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGameMode.generated.h"

UCLASS()
class ONLINELEARNINGKIT_API AMyGameMode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void SpawnPlatforms();

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<class ATimePlatform> PlatformClass; // 스폰할 클래스 설정
};
