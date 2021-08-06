// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGraphExample.h"
#include "Async/TaskGraphInterfaces.h"

#include "HAL/ThreadManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATaskGraphExample::ATaskGraphExample()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATaskGraphExample::BeginPlay()
{
	Super::BeginPlay();
	
	/*MainIdThread = FPlatformTLS::GetCurrentThreadId();
	FString NameThread = FThreadManager::GetThreadName(MainIdThread);
	UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork NameThread - %s ID - %d"), *NameThread, MainIdThread);

	UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork NumberOfCores - %d"), FPlatformMisc::NumberOfCores());
	UE_LOG(LogTemp, Error, TEXT("FTask_SimpleWork NumberOfCoresIncludingHyperthreads - %d"), FPlatformMisc::NumberOfCoresIncludingHyperthreads());*/
	
	TaskDelegate.BindUFunction(this, FName("OnWorkDone"));
	AThreadExampleGameModeBase *myGameMode = Cast<AThreadExampleGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	myCurrentTask = TGraphTask<FTask_CounterWork>::CreateTask(nullptr, ENamedThreads::AnyThread).ConstructAndHold(TaskDelegate, myGameMode, &Counter, MainIdThread);
}

// Called every frame
void ATaskGraphExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATaskGraphExample::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATaskGraphExample::StartAsyncWork()
{
	if (myCurrentTask)
	{
		//wait unlock task
		if (!myCurrentTask->GetCompletionEvent().IsValid())
		{
			myCurrentTask->Unlock();
		}		
	}
	else
	{
		//Fire and Forget
		AThreadExampleGameModeBase *myGameMode = Cast<AThreadExampleGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		TGraphTask<FTask_CounterWork>::CreateTask(nullptr, ENamedThreads::AnyThread).ConstructAndDispatchWhenReady(TaskDelegate, myGameMode, &Counter, MainIdThread);
	}		
}

void ATaskGraphExample::OnWorkDone(int32 Result)
{
	myCurrentTask = nullptr;
	OnWorkDone_BP(Result);
}

void ATaskGraphExample::OnWorkDone_BP_Implementation(int32 Result)
{
	//BP
}