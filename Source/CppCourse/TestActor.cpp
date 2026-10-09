// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MySphereCollision"));
	RootComponent = MyCollision;

	MyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMeshComponent"));
	MyMeshComponent->SetupAttachment(MyCollision);

	MySpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MySpringArm"));
	MySpringArm->TargetArmLength = 500.0f;
	MySpringArm->SetupAttachment(MyCollision);

	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MyCamera"));
	MyCamera->SetupAttachment(MySpringArm);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MyMeshAsset(TEXT("/Game/cc0_gold_coin_blank.cc0_gold_coin_blank"));
	MyMeshComponent->SetStaticMesh(MyMeshAsset.Object);

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(MyRotationTimer, this, &ATestActor::Func_Rotation, 0.016f, true);
	
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckDistance();
}

void ATestActor::printSomething()
{
}

void ATestActor::Func_Rotation()
{
	FRotator NewRotation = MyMeshComponent->GetRelativeRotation();
	NewRotation.Yaw += RotationSpeed * 0.016f;
	MyMeshComponent->SetRelativeRotation(NewRotation);

}

void ATestActor::CheckDistance()
{
	ACppCourseCharacter* MyPlayerCharacter = Cast<ACppCourseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	FVector GetCoinLocation = GetActorLocation();

	if (GetDistanceTo(MyPlayerCharacter) < 200.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(MyRotationTimer);
	}
	else
	{
		Func_Rotation();
	}
}

