// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaskGraphExample.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskDelegate_OnWorkDone, int32, OutputResult);

UCLASS()
class THREADEXAMPLE_API ATaskGraphExample : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATaskGraphExample();

	UPROPERTY(BlueprintAssignable)
	FTaskDelegate_OnWorkDone TaskDelegate_OnWorkDone;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGraphEventArray myGraphArray;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGraphEventRef myCurrentTask;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 SimpleWork(int32 Value);

	UFUNCTION(BlueprintCallable)
	void StartAsyncWork();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 counter = 0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 MainIdThread = 0;
};
