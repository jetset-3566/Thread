// Fill out your copyright notice in the Description page of Project Settings.


#include "AsynTaskExample.h"
#include "HAL/ThreadManager.h"
class ExampleAutoDeleteAsyncTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<ExampleAutoDeleteAsyncTask>;

	int32 ExampleData;
	FAsyncTaskDelegate_OnWorkDone AsyncTaskDelegate_OnWorkDone;
	int32* SimpleOutput;

	ExampleAutoDeleteAsyncTask(int32 InExampleData, FAsyncTaskDelegate_OnWorkDone InAsyncTaskDelegate_OnWorkDone, int32 *InSimpleOutput)
		: ExampleData(InExampleData),
		AsyncTaskDelegate_OnWorkDone(InAsyncTaskDelegate_OnWorkDone),
		SimpleOutput(InSimpleOutput)
	{
	}

	void DoWork()
	{
		
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
		AsyncTaskDelegate_OnWorkDone.Broadcast(result);
		
		uint32 MainIdThread = FPlatformTLS::GetCurrentThreadId();
		FString NameThread = FThreadManager::GetThreadName(MainIdThread);
		UE_LOG(LogTemp, Error, TEXT("ExampleAutoDeleteAsyncTask NameThread - %s ID - %d"), *NameThread, MainIdThread);

		UE_LOG(LogTemp, Error, TEXT("ExampleAutoDeleteAsyncTask NumberOfCores - %d"), FPlatformMisc::NumberOfCores());
		UE_LOG(LogTemp, Error, TEXT("ExampleAutoDeleteAsyncTask NumberOfCoresIncludingHyperthreads - %d"), FPlatformMisc::NumberOfCoresIncludingHyperthreads());
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(ExampleAutoDeleteAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};

// Sets default values
AAsynTaskExample::AAsynTaskExample()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAsynTaskExample::BeginPlay()
{
	Super::BeginPlay();
	//FEvent
	
	AsyncTask(ENamedThreads::GameThread, [&]()
		{
		UE_LOG(LogTemp, Warning, TEXT("AAsynTaskExample::BeginPlay  AsyncTask"));
		});
}

// Called every frame
void AAsynTaskExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAsynTaskExample::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

//	myAutoDeleteAsyncTask.Abandon();
}

void AAsynTaskExample::StartExample(bool bIsBackGroundTask)
{
	
	//if (!myAutoDeleteAsyncTask.Task)
	{
		if (bIsBackGroundTask)
		{
			// start an example job
			(new FAutoDeleteAsyncTask<ExampleAutoDeleteAsyncTask>(5, AsyncTaskDelegate_OnWorkDone, &counter))->StartBackgroundTask();
		}
		else
		{
			// do an example job now, on this thread
			(new FAutoDeleteAsyncTask<ExampleAutoDeleteAsyncTask>(5, AsyncTaskDelegate_OnWorkDone, &counter))->StartSynchronousTask();
		}
	}
}