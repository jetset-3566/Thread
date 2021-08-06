// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"

#include "TaskGraphExample.generated.h"

DECLARE_DELEGATE_OneParam(FTaskDelegate_OnWorkDone, int32 OutputResult)
	  
class FTask_FinishWork
{
	FTaskDelegate_OnWorkDone TaskDelegate_OnWorkDone;
	AThreadExampleGameModeBase *GameModeRef;
	int32 Result;	
	
public:
	FTask_FinishWork(FTaskDelegate_OnWorkDone InTaskDelegate_OnWorkDone, AThreadExampleGameModeBase* InGameModeRef, int32 InResult) 
		: TaskDelegate_OnWorkDone(InTaskDelegate_OnWorkDone),
		GameModeRef(InGameModeRef), 
		Result(InResult)
	{
		
	}
	~FTask_FinishWork()
	{
		
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_FinishWork, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread()
	{
		//run task in game thread for delegate correctly work
		return ENamedThreads::GameThread;		
	}

	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::FireAndForget; }
	
	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		check(IsInGameThread());
		if (TaskDelegate_OnWorkDone.IsBound())
		{
			TaskDelegate_OnWorkDone.Execute(Result);	
		}
		
	}
};

class FTask_CounterWork
{
	FTaskDelegate_OnWorkDone TaskDelegate_OnWorkDone;
	AThreadExampleGameModeBase *GameModeRef;
	int32 *SimpleOutput;
	uint32 MainIdThread;
	
public:
	FTask_CounterWork(FTaskDelegate_OnWorkDone InTaskDelegate_OnWorkDone, AThreadExampleGameModeBase* InGameModeRef, int32 *InSimpleOutput, uint32 InMainIdThread) 
		: TaskDelegate_OnWorkDone(InTaskDelegate_OnWorkDone),
		GameModeRef(InGameModeRef), 
		SimpleOutput(InSimpleOutput),
		MainIdThread(InMainIdThread)
	{
		UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork Cons"));//Use breakPoint for understand where created Task (use #pragma optimize ())

	}
	~FTask_CounterWork()
	{
		UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork Dest"));//Use breakPoint for understand where Task deleted
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_CounterWork, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread()
	{
		//backGround run task
		FAutoConsoleTaskPriority myTaskPriority(
			TEXT("TaskGraph.TaskPriorities.LoadFileToString"),
			TEXT("Task and thread priority for file loading."),
			ENamedThreads::BackgroundThreadPriority,
			ENamedThreads::NormalTaskPriority,
			ENamedThreads::NormalTaskPriority
		);
		return myTaskPriority.Get();		
	}

	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::FireAndForget; }
	

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		int32 i = 0;
		while (i < 500)
		{
			FPlatformProcess::Sleep(0.002f); 
			*SimpleOutput = (*SimpleOutput + i);
			i++;
			//GameModeRef->SimpleCounter we can take other object and change (not forget critical section)
		}
		
		//TaskDelegate_OnWorkDone.ExecuteIfBound(result);//Bad practice try use ue delegate in other thread (not inGameThread)
		TGraphTask<FTask_FinishWork>::CreateTask(NULL, CurrentThread).ConstructAndDispatchWhenReady(TaskDelegate_OnWorkDone, GameModeRef, *SimpleOutput);
	}
};

UCLASS()
class THREADEXAMPLE_API ATaskGraphExample : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATaskGraphExample();

	FTaskDelegate_OnWorkDone TaskDelegate;					  
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual  void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void StartAsyncWork();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Counter = 0;
	uint32 MainIdThread = 0;

	TGraphTask<FTask_CounterWork> *myCurrentTask;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnWorkDone_BP(int32 Result);
	UFUNCTION()
	void OnWorkDone(int32 Result);
};	   