#include "TimePlatform.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"  // 플레이어 감지를 위해 필요

ATimePlatform::ATimePlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneRoot);
	
	bIsSolid = true;

	MoveDirection = FVector::ZeroVector;
	MoveSpeed = 0.0f;
	MoveRange = 0.0f;

	// **충돌 감지 (플랫폼 위에 올라섰을 때)**
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->OnComponentHit.AddDynamic(this, &ATimePlatform::OnPlatformStepped);
}

void ATimePlatform::BeginPlay()
{
	Super::BeginPlay();
	SetSolidState(true);
	
	// 타이머 시작, 주기에 따라 이동 패턴 변경
	GetWorld()->GetTimerManager().SetTimer(
		MovementChangeTimer,
		this,
		&ATimePlatform::RandomizeMovement,
		5.0f,
		true // 무한 반복
	);
	RandomizeMovement();
}

void ATimePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 이동 로직
	FVector NewLocation = GetActorLocation() + (MoveDirection * MoveSpeed * DeltaTime);
	SetActorLocation(NewLocation);
}

void ATimePlatform::ToggleState()
{
	bIsSolid = !bIsSolid;
	SetSolidState(bIsSolid);
	UGameplayStatics::PlaySoundAtLocation(this, ChangeStateSound, GetActorLocation());// 효과음 재생
}

void ATimePlatform::SetSolidState(bool bSolid)
{
	if (bSolid)
	{
		// Solid 상태: 콜리전 활성화 + 파란 머티리얼
		StaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
		StaticMesh->SetMaterial(0, SolidMaterial);
		UE_LOG(LogTemp, Warning, TEXT("Platform is now SOLID"));
	}
	else
	{
		// Intangible 상태: 콜리전 비활성화 + 빨간 머티리얼
		StaticMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		StaticMesh->SetMaterial(0, IntangibleMaterial);
		UE_LOG(LogTemp, Warning, TEXT("Platform is now INTANGIBLE"));
		UGameplayStatics::PlaySoundAtLocation(this, ChangeStateSound, GetActorLocation());
	}
}

void ATimePlatform::RandomizeMovement()
{
	// 랜덤 이동 방향
	MoveDirection = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 0).GetSafeNormal();

	// 랜덤한 속도와 범위
	MoveSpeed = FMath::RandRange(50.0f, 500.0f);
	MoveRange = FMath::RandRange(100.0f, 300.0f);
}

void ATimePlatform::OnPlatformStepped(UPrimitiveComponent* HitComponent, AActor* OtherActor,
									  UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(ACharacter::StaticClass())) // 플레이어가 밟았을 때
	{
		UE_LOG(LogTemp, Warning, TEXT("✅Player stepped on the platform! Timer started."));
		GetWorld()->GetTimerManager().SetTimer(
			PlayerDetectionTimer,
			this,
			&ATimePlatform::DelayedBecomeIntangible,
			2.0f,
			false);
		UGameplayStatics::PlaySoundAtLocation(this, ChangeStateSound, GetActorLocation());
	}
}

void ATimePlatform::DelayedBecomeIntangible() // x초 뒤에 비물질 상태로 변환.
{
	SetSolidState(false);
	UE_LOG(LogTemp, Warning, TEXT("Platform is now intangible after 5 seconds!"));
	GetWorld()->GetTimerManager().SetTimer(
		RestoreSolidTimer,
		this,
		&ATimePlatform::DelayedBecomeSolid,
		3.0f,
		false);	
}

void ATimePlatform::DelayedBecomeSolid()
{
	SetSolidState(true);
}

