// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnableThreadExample.h"
#include "RunnableExample.h"


FRunnableThreadExample::FRunnableThreadExample(int32 CalcInput, ARunnableExample* OwnerActor)
{
	if (CalcInput > 0 && OwnerActor)
	{
		Calculation = CalcInput;
		CurrentThreadActor = OwnerActor;		
	}
}

FRunnableThreadExample::~FRunnableThreadExample()
{
}

bool FRunnableThreadExample::Init()
{
	bIsStopThread = false;
	CalcCount = 0;
	return true;
}

uint32 FRunnableThreadExample::Run()
{
	while (!bIsStopThread)
	{
		if (CalcCount < Calculation && CurrentThreadActor)
		{
			
			//FEvent wait - Condition Variable
			//myCriticalSection.TryLock();
			CurrentCalculation += FMath::RandRange(20, 400);
			CurrentCalculation *= FMath::RandRange(2,500);
			CurrentCalculation -= FMath::RandRange(10, 500);

			CurrentThreadActor->ThreadCalcQueue.Enqueue(CurrentCalculation); 

			UE_LOG(LogTemp, Error, TEXT("FRunnableThreadExample::Run ownerId - %s  DATA - %d "), *CurrentThreadActor->GetName(), CurrentCalculation);

			//myCriticalSection.Unlock();

			CalcCount++;

			FPlatformProcess::Sleep(0.002f);

			CurrentThreadActor->CurrentCalc = CalcCount;

			if (CurrentThreadActor->myGameMode)
			{
				
				CurrentThreadActor->myGameMode->SetGlobalString("Thread Work");
				CurrentThreadActor->myGameMode->SetGlobalValue(CurrentCalculation);
			}
		}
		else
		{
			bIsStopThread = true;
		}
	}

	return 0;
}

void FRunnableThreadExample::Stop()
{

}

void FRunnableThreadExample::Exit()
{

}
