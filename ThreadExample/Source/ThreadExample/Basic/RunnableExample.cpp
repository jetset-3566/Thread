// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnableExample.h"
#include "Kismet/GameplayStatics.h"

ARunnableExample::ARunnableExample()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARunnableExample::InitCalculation(int32 Calc)
{
	CalcThread = new FRunnableThreadExample(Calc, this);
	CurrentRunningThread = FRunnableThread::Create(CalcThread, TEXT("ActorCalcl Thread"), 0, EThreadPriority::TPri_Highest);

	FTimerDelegate Dead_TimerDelegate;
	Dead_TimerDelegate.BindLambda([&]()
	{
		this->Destroy();
	});

	FTimerHandle myHandle;
	GetWorld()->GetTimerManager().SetTimer(myHandle, Dead_TimerDelegate, DiedTimer, false);
}

void ARunnableExample::BeginPlay()
{
	Super::BeginPlay();
}
void ARunnableExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARunnableExample::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (CurrentRunningThread && CalcThread)
	{		
		CalcThread->bIsStopThread = true;//Commit safe exit thread when actor destroy to check thread still running
		CurrentRunningThread->WaitForCompletion();
	}
}

int32 ARunnableExample::GetCalcValue()
{
	return CurrentCalc.GetValue();
}

