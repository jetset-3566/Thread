// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGraphExample_Track.h"
#include "Async/TaskGraphInterfaces.h"

#include "HAL/ThreadManager.h"


class FTask_FinishWork_Track//dont use copy another name of class task, i spend 3 hour to find this issie, kill me plz
{
	FTaskDelegate_OnWorkDone TaskDelegate_OnWorkDone;	
	int32 Result;//Use pointer * for do not take the value of the variable but the variable itself
	
public:
	FTask_FinishWork_Track( FTaskDelegate_OnWorkDone InTaskDelegate_OnWorkDone, int32 InResult) 
		: TaskDelegate_OnWorkDone(InTaskDelegate_OnWorkDone),
		Result(InResult)
		
	{
		
	}
	~FTask_FinishWork_Track(){}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_FinishWork_Track, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread()
	{
		//run task in game thread for delegate correctly work
		return ENamedThreads::GameThread;		
	}

	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }
	
	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		if(TaskDelegate_OnWorkDone.IsBound())
		{
			TaskDelegate_OnWorkDone.Execute(Result);	
		}		
	}
};

class FTask_Counter_Track
{
	int32 *Result;	
	FCriticalSection *CounterMutex;
public:
	FTask_Counter_Track(int32 *InResult, FCriticalSection *InCounterMutex) 
		: Result(InResult),
		CounterMutex(InCounterMutex)
	{
		
	}
	~FTask_Counter_Track(){}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_Counter_Track, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread()
	{
		//backGround run task
		FAutoConsoleTaskPriority TaskPriority(
			TEXT("TaskGraph.TaskPriorities.LoadFileToString"),
			TEXT("Task and thread priority for file loading."),
			ENamedThreads::BackgroundThreadPriority,
			ENamedThreads::NormalTaskPriority,
			ENamedThreads::NormalTaskPriority
		);
		return TaskPriority.Get();		
	}

	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	static void AnyThreadWork()
	{
		//we can create function in class FTask for used
	}
	
	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		for (int i = 0; i < 50; ++i)
		{
			FPlatformProcess::Sleep(0.02f);
			
			FScopeLock ScopedLock(CounterMutex);
			*Result = *Result + 1;
		}

		//AnyThreadWork();
	}
};

ATaskGraphExample_Track::ATaskGraphExample_Track()
{
	PrimaryActorTick.bCanEverTick = true;
}
void ATaskGraphExample_Track::BeginPlay()
{
	Super::BeginPlay();
}
void ATaskGraphExample_Track::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ATaskGraphExample_Track::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATaskGraphExample_Track::StartAsyncTask()
{
	FGraphEventArray Prerequisites;//list task for FinishWork task
	
	for (int i = 0; i < 3; ++i)
	{
		//counter task start work when create it
		FGraphEventRef CounterTask = TGraphTask<FTask_Counter_Track>::CreateTask(nullptr, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(&Counter,&CounterMutex);
		Prerequisites.Add(CounterTask);
	}
	
	TaskDelegate_OnWorkDone.BindUFunction(this,"OnWorkDone");
	
	TGraphTask<FTask_FinishWork_Track>::CreateTask(&Prerequisites, ENamedThreads::AnyThread).ConstructAndDispatchWhenReady(TaskDelegate_OnWorkDone, Counter);
}

void ATaskGraphExample_Track::OnWorkDone(int32 Result)
{
	OnWorkDone_BP(Counter);//Take variable in ATaskGraphExapleTrack object
	
	//OnWorkDone_BP(Result); for if we use pointer when create task FinishWork
}

void ATaskGraphExample_Track::OnWorkDone_BP_Implementation(int32 Result)
{
	//BP
}

