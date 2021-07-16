// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsynTaskExample.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncTaskDelegate_OnWorkDone, int32, OutputResult);

UCLASS()
class THREADEXAMPLE_API AAsynTaskExample : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsynTaskExample();

	UPROPERTY(BlueprintAssignable)
	FAsyncTaskDelegate_OnWorkDone AsyncTaskDelegate_OnWorkDone;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 counter = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason);
	UFUNCTION(BlueprintCallable)
	void StartExample(bool bIsBackGroundTask = true);
};
