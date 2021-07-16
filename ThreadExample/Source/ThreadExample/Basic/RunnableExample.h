// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "HAL/Runnable.h"
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

	void PrintCalcData();
	int32 ProcesserCalculation;
	
	class FRunnableThreadExample *CalcThread = nullptr;
	FRunnableThread *CurrentRunningThread = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason);

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentCalc;

	TQueue<int32> ThreadCalcQueue;//потокобезопасна€ очередьё,?

	UFUNCTION()
	void KillRunnableExample();

	UPROPERTY()
	AThreadExampleGameModeBase *myGameMode = nullptr;
};
