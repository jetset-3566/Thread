// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnableThreadExample.h"
#include "RunnableExample.h"


FRunnableThreadExample::FRunnableThreadExample(int32 CalcInput, ARunnableExample* OwnerActor)
{
	Calculation = CalcInput;
	CurrentThreadActor = OwnerActor;
}

FRunnableThreadExample::~FRunnableThreadExample()
{
}

bool FRunnableThreadExample::Init()
{
	return true;
}

uint32 FRunnableThreadExample::Run()
{
	int32 CalcCount = 0;
	while (!bIsStopThread)
	{
		if (CalcCount < Calculation)
		{
			CurrentCalculation += FMath::RandRange(20, 400);
			CurrentCalculation *= FMath::RandRange(2,500);
			CurrentCalculation -= FMath::RandRange(10, 500);

			CurrentThreadActor->CurrentCalc.Set(CurrentCalculation);

			//UE_LOG(LogTemp, Error, TEXT("FRunnableThreadExample::Run ownerId - %s  DATA - %d "), *CurrentThreadActor->GetName(), CurrentCalculation);
			
			FPlatformProcess::Sleep(0.002f);
			CalcCount++;
		}
		else
		{
			bIsStopThread = true;
		}
	}

	return 1;
}


