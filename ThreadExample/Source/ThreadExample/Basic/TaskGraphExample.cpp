// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGraphExample.h"
#include "Async/TaskGraphInterfaces.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"

#include "HAL/ThreadManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include <thread>
#include <chrono>

//DECLARE_DELEGATE_OneParam(FTaskDelegate_OnWorkDone, int32 OutputResult)

// Sets default values
ATaskGraphExample::ATaskGraphExample()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FAutoConsoleTaskPriority myTaskPriority(
	TEXT("TaskGraph.TaskPriorities.LoadFileToString"),
	TEXT("Task and thread priority for file loading."),
	ENamedThreads::HighThreadPriority,
	ENamedThreads::NormalTaskPriority,
	ENamedThreads::HighTaskPriority
);

class FTask_SimpleWork
{
	FTaskDelegate_OnWorkDone TaskDelegate_OnWorkDone;
	//int32 SimpleArgument;
	int32 *SimpleOutput;
	uint32 MainIdThread;
	
public:
	FTask_SimpleWork(FTaskDelegate_OnWorkDone InTaskDelegate_OnWorkDone, /*int32 InSimpleArgument,*/ int32 *InSimpleOutput, uint32 InMainIdThread) 
		: TaskDelegate_OnWorkDone(InTaskDelegate_OnWorkDone),
		/*SimpleArgument(InSimpleArgument),*/ 
		SimpleOutput(InSimpleOutput), MainIdThread(InMainIdThread)
	{

	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTaskDelegate_OnWorkDone, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread() { return myTaskPriority.Get(); }

	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::FireAndForget; }

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		//check(IsInGameThread());

		int32 result;
		uint64 i = 0;
		while (i < 500)
		{
			FPlatformProcess::Sleep(0.002f); 
			//std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
			*SimpleOutput = (*SimpleOutput + 5);
			result = *SimpleOutput;


			i++;
		}
		TaskDelegate_OnWorkDone.Broadcast(result);

		FString NameThread = FThreadManager::GetThreadName(FPlatformTLS::GetCurrentThreadId());
		UE_LOG(LogTemp, Warning, TEXT("FTask_SimpleWork NameThread - %s ID - %d"), *NameThread, FPlatformTLS::GetCurrentThreadId());

		if (MainIdThread == FPlatformTLS::GetCurrentThreadId())
		{
			UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork MainThread - %d TaskThread - %d"), MainIdThread, FPlatformTLS::GetCurrentThreadId());
		}

		
	}
};

// Called when the game starts or when spawned
void ATaskGraphExample::BeginPlay()
{
	Super::BeginPlay();
	
	MainIdThread = FPlatformTLS::GetCurrentThreadId();
	FString NameThread = FThreadManager::GetThreadName(MainIdThread);
	UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork NameThread - %s ID - %d"), *NameThread, MainIdThread);

	UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork NumberOfCores - %d"), FPlatformMisc::NumberOfCores());
	UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork NumberOfCoresIncludingHyperthreads - %d"), FPlatformMisc::NumberOfCoresIncludingHyperthreads());

	
	
}

// Called every frame
void ATaskGraphExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ATaskGraphExample::SimpleWork(int32 Value)
{
	counter = counter + Value;
	if (counter>50000)
	{
		return counter;
	}
	SimpleWork(Value);
	return counter;
}

void ATaskGraphExample::StartAsyncWork()
{
	myCurrentTask = TGraphTask<FTask_SimpleWork>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(TaskDelegate_OnWorkDone, &counter, MainIdThread);
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}

