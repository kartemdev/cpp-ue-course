// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "CppCourseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TestActor.generated.h"


USTRUCT(BlueprintType)
struct FMyStructure
{
	GENERATED_BODY()
};

UCLASS()
class CPPCOURSE_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TankInfo")
	float Health = 100.f;

	UFUNCTION(BlueprintCallable)
	void printSomething();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MyMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* MySpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* MyCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* MyCollision;

	FTimerHandle MyRotationTimer;

	UPROPERTY(EditAnywhere, Category="Rotation")
	float RotationSpeed = 90.f;

	UFUNCTION()
	void Func_Rotation();

	UFUNCTION()
	void CheckDistance();
};
