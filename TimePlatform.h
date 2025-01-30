#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimePlatform.generated.h"

UCLASS()
class ONLINELEARNINGKIT_API ATimePlatform : public AActor
{
	GENERATED_BODY()

public:
	ATimePlatform();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	// 컴포넌트
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* StaticMesh;

	// 머터리얼
	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* SolidMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* IntangibleMaterial;

	// 오디오
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* ChangeStateSound;

	// 타이머 핸들
	FTimerHandle StateTimerHandle;
	FTimerHandle MovementChangeTimer;
	FTimerHandle PlayerDetectionTimer;
	FTimerHandle RestoreSolidTimer;

	// 랜덤 이동/회전 변수
	FVector MoveDirection;
	float MoveSpeed;
	float MoveRange;
	
	bool bIsSolid = true; // 현재 상태
	
	// 상태 전환 주기 (초 단위)
	UPROPERTY(EditAnywhere, Category = "Timer")
	float StateChangeInterval = 5.0f;
	
	void ToggleState();
	void SetSolidState(bool bSolid); 	// 머티리얼 및 콜리전 설정
	void RandomizeMovement();
	void DelayedBecomeIntangible();	// ?초 후 비물질화 메서드
	void DelayedBecomeSolid(); // ?초 후 물질화 메서드
	
	// **플레이어가 플랫폼을 밟았을 때 감지**
	UFUNCTION()
	void OnPlatformStepped(UPrimitiveComponent* HitComponent, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};