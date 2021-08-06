// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaskGraphExample_Track.generated.h"

DECLARE_DELEGATE_OneParam(FTaskDelegate_OnWorkDone, int32 OutputResult)

UCLASS()
class THREADEXAMPLE_API ATaskGraphExample_Track : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATaskGraphExample_Track();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintCallable)
	void StartAsyncTask();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Counter = 0;
	

	FCriticalSection CounterMutex;

	FTaskDelegate_OnWorkDone TaskDelegate_OnWorkDone;
	UFUNCTION()
	void OnWorkDone(int32 Result);
	UFUNCTION(BlueprintNativeEvent)
	void OnWorkDone_BP(int32 Result);
};


