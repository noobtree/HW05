// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomTranslateActor.generated.h"

// Forward Declaration
class UInputMappingContext;
class UInputAction;
class UInputComponent;

UCLASS()
class HW05_API ARandomTranslateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomTranslateActor();

	// StaticMesh 에셋을 설정할 수 있는 컴포넌트
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> meshComponent;

	// Input Mapping Context 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essential Feature")
	TObjectPtr<UInputMappingContext> inputMappingContextAsset;

	// Input Action 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essential Feature")
	TObjectPtr<UInputAction> inputActionAsset;

#pragma region Initial Setting

	// 초기 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initial Setting")
	FVector initialLocation = FVector(0, 50, 0);
	// 초기 회전
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initial Setting")
	FRotator initialRotationEuler = FRotator::MakeFromEuler({ 0, 0, 0 });

#pragma endregion

	// 이동 속도 (cm/s)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essential Feature")
	float locationSpeedPerSec = 150;

	// 회전 속도 (deg/s)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essential Feature")
	float roationSpeedPerSec = 90;

	// 이동 시도 횟수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Essential Feature")
	int32 tryCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Challenge Feature")
	int32 actualCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Challenge Feature")
	// 총 이동 거리
	float totalDistance = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPressKey();

	UFUNCTION(BlueprintNativeEvent)		// Blueprint에서 수정 가능하도록 매크로 추가
	void Move();	// 함수 이름 설정
	virtual void Move_Implementation();	// Move 함수 기초 정의 작성

	UFUNCTION(BlueprintNativeEvent)		// Blueprint에서 수정 가능하도록 매크로 추가
	void Turn();	// 함수 이름 설정
	virtual void Turn_Implementation();	// Turn 함수 기초 정의 작성
};
