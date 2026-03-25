// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomTranslateActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

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

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (playerController != nullptr) {
		// enable to get input at this actor
		EnableInput(playerController);

		if (ULocalPlayer* player = playerController->GetLocalPlayer()) {
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
				InputSystem->AddMappingContext(inputMappingContextAsset, 0);
			}
		}

		if (UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
			// bind InputAction Event
			enhancedInputComponent->BindAction(inputActionAsset, ETriggerEvent::Started, this, &ARandomTranslateActor::OnPressKey);
		}
	}

	SetActorLocation(initialLocation);
	SetActorRotation(initialRotationEuler);
	tryCount = 0;
}

// Called every frame
void ARandomTranslateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARandomTranslateActor::OnPressKey()
{
	check(GEngine != nullptr);
	// increase try count
	++tryCount;
	
	// Check 50% probability
	if (tryCount < 10) {
		if (FMath::FRand() < 0.5f) {
			// increase actual count
			++actualCount;

			FVector before = GetActorLocation();

			// apply move
			Move();
			// apply turn
			Turn();

			FVector after = GetActorLocation();

			float deltaDistance = FVector::Distance(before, after);

			totalDistance += deltaDistance;

			FString debugMessage = "Location : " + GetActorLocation().ToString() + "    |    Rotation : " + GetActorRotation().Euler().ToString();
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, debugMessage);
		}

	}

	// if try count is over 10, print debug mesage
	if (tryCount >= 10) {
		FString resultMessage = "Move Count : " + FString::FormatAsNumber(actualCount) + "  |  total Move Distance : " + FString::SanitizeFloat(totalDistance);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, resultMessage);
	}
	return;
}

void ARandomTranslateActor::Move_Implementation()
{
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Move"));

	// create radomized vector
	FVector deltaDirection = FMath::VRand();

	// calculate new location
	float deltaTime = GetWorld()->DeltaTimeSeconds;
	FVector newLocation = GetActorLocation() + (locationSpeedPerSec * deltaTime * deltaDirection);

	// set move option
	//bool bSweep = true;
	bool bSweep = false;
	FHitResult sweepResult;

	// apply move
	if (SetActorLocation(newLocation, bSweep, &sweepResult) == false) {
		// hit somewhere during sweep, modify actor location
		SetActorLocation(sweepResult.Location);
	}
}

void ARandomTranslateActor::Turn_Implementation()
{
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Turn"));

	// create radomized vector
	FVector deltaEuler = FMath::VRand();

	// calculate deltaEuler
	float deltaTime = GetWorld()->DeltaTimeSeconds;
	deltaEuler *= roationSpeedPerSec * deltaTime;

	// create FRotator from euler
	FRotator deltaRotator = FRotator::MakeFromEuler(deltaEuler);

	// apply turn
	AddActorLocalRotation(deltaRotator);
}

