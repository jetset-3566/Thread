// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "HAL/RunnableThread.h"
#include "RunnableThreadExample.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"

#include "RunnableExample.generated.h"

UCLASS()
class THREADEXAMPLE_API ARunnableExample : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARunnableExample();

	UFUNCTION(BlueprintCallable)
	void InitCalculation(int32 Calc);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class FRunnableThreadExample *CalcThread = nullptr;
	FRunnableThread *CurrentRunningThread = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason);

	FThreadSafeCounter CurrentCalc;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DiedTimer = 10.0f;;
	
	UFUNCTION(BlueprintCallable)
	int32 GetCalcValue();
};
