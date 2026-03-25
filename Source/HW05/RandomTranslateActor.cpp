// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomTranslateActor.h"

// Sets default values
ARandomTranslateActor::ARandomTranslateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create StaticMesh Component Instance
	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("ActorMesh"));
}

// Called when the game starts or when spawned
void ARandomTranslateActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(initialLocation);
	SetActorRotation(initialRotationEuler);
	count = 0;
}

// Called every frame
void ARandomTranslateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	check(GEngine != nullptr);
	if (count < 10) {
		++count;
		Move(DeltaTime);
		Turn(DeltaTime);

		FString debugMessage = "Location : " + GetActorLocation().ToString() + "    |    Rotation : " + GetActorRotation().Euler().ToString();
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, debugMessage);
	}
}

void ARandomTranslateActor::Move_Implementation(float DeltaTime)
{
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Move"));

	// create radomized vector
	FVector deltaDirection = FMath::VRand();

	// calculate new location
	FVector newLocation = GetActorLocation() + (locationSpeedPerSec * DeltaTime * deltaDirection);
	
	// set move option
	bool bSweep = true;
	FHitResult sweepResult;

	// apply move
	if (SetActorLocation(newLocation, bSweep, &sweepResult) == false) {
		// hit somewhere during sweep, modify actor location
		SetActorLocation(sweepResult.Location);
	}
}

void ARandomTranslateActor::Turn_Implementation(float DeltaTime)
{
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Turn"));

	// create radomized vector
	FVector deltaEuler = FMath::VRand();

	// create FRotator from euler
	FRotator deltaRotator = FRotator::MakeFromEuler(deltaEuler);

	// apply turn
	AddActorLocalRotation(deltaRotator);
}

