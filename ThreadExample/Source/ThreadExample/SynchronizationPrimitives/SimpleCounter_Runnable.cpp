// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleCounter_Runnable.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"

FSimpleCounter_Runnable::FSimpleCounter_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor, bool VariableMode)
{
	GameMode_Ref = OwnerActor;
	ThreadColor = Color;
	bIsUseSafeVariable = VariableMode;
	UE_LOG(LogTemp, Warning, TEXT("FNameGeneratorLogic_Runnable::Constructor "));
}

FSimpleCounter_Runnable::~FSimpleCounter_Runnable()
{
	UE_LOG(LogTemp, Warning, TEXT("FNameGeneratorLogic_Runnable::Destructor "));
}

bool FSimpleCounter_Runnable::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("FNameGeneratorLogic_Runnable::Init "));
	return true;	
}
//#pragma optimize ("", off)
uint32 FSimpleCounter_Runnable::Run()
{
	//FEvent
	if (GameMode_Ref->SimpleCounterEvent)
	{
		GameMode_Ref->SimpleCounterEvent->Wait(10000);
		if (GameMode_Ref->SimpleCounterEvent)//check before use again after 10 second wait maybe event = null 
		{
			GameMode_Ref->SimpleCounterEvent->Wait(10000); //we can use event again - one trigger = one unlock guard or ReturnSynchEventToPool 	
		}		
	}

	//FScopedEvent
	if (GameMode_Ref->bIsUseFSScopedEvent)
	{
		{
			FScopedEvent SimpleCounterScopedEvent;
			GameMode_Ref->SendRef_ScopedEvent(SimpleCounterScopedEvent);
			//wait here - destructor start here
		}
	}
	
	
	if (bIsUseSafeVariable)
	{
		while (!bIsStopThreadSafe)
		{
			CounterSafe.Increment();
		}
	}
	else
	{
		while (!bIsStopThread)
		{
			Counter++;
			//FPlatformProcess::Sleep(0.0200f);//for chance exit loop we can add sleep
		}
	}
	
	return 0;	
}
//#pragma optimize ("", on)
void FSimpleCounter_Runnable::Stop()
{
	UE_LOG(LogTemp, Warning, TEXT("FNameGeneratorLogic_Runnable::Stop "));
	bIsStopThread = true;
	bIsStopThreadSafe = true;
}

void FSimpleCounter_Runnable::Exit()
{
	GameMode_Ref = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("FNameGeneratorLogic_Runnable::Exit "));
}
