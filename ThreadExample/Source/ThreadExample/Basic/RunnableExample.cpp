// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnableExample.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARunnableExample::ARunnableExample()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ARunnableExample::InitCalculation(int32 Calc)
{
	if (Calc > 0)
	{
		CalcThread = new FRunnableThreadExample(Calc, this);
		CurrentRunningThread = FRunnableThread::Create(CalcThread, TEXT("Calc Thread"), 0, EThreadPriority::TPri_Highest);

		UE_LOG(LogTemp, Error, TEXT("ARunnableExample::InitCalculation nameThread - %s"), *CurrentRunningThread->GetThreadName());
		FTimerDelegate Dead_TimerDelegate;
		
		Dead_TimerDelegate.BindLambda([&]()
		{
			this->Destroy();
		});

		FTimerHandle myHandle;
		GetWorld()->GetTimerManager().SetTimer(myHandle, Dead_TimerDelegate, 10.f, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ARunnableExample::InitCalculation NOTzero"));
	}
}

// Called when the game starts or when spawned
void ARunnableExample::BeginPlay()
{
	Super::BeginPlay();

	myGameMode = Cast<AThreadExampleGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ARunnableExample::PrintCalcData()
{
	if (!ThreadCalcQueue.IsEmpty() && ThreadCalcQueue.Dequeue(ProcesserCalculation))
	{
		UE_LOG(LogTemp, Error, TEXT("ARunnableExample::PrintCalcData DATA - %d "), ProcesserCalculation);
	}
}

// Called every frame
void ARunnableExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PrintCalcData();
}

void ARunnableExample::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (CurrentRunningThread && CalcThread)
	{
		CurrentRunningThread->Suspend(true);//
		CalcThread->bIsStopThread = true;
		CurrentRunningThread->Suspend(false);

		CurrentRunningThread->WaitForCompletion();//join?

		delete CalcThread;
	}
}

void ARunnableExample::KillRunnableExample()
{
	this->Destroy();
}

