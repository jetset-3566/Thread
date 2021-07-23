// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleAtomic_Runnable.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"

FSimpleAtomic_Runnable::FSimpleAtomic_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor, uint32 NeedIteration, bool SeparateLogic, bool bIsUseAtomic)
{
	ThreadColor = Color;
	GameMode_Ref = OwnerActor;
	NumberIteration = NeedIteration;
	SeparateLogicFlag = SeparateLogic;
	bIsUseAtomicFlag = bIsUseAtomic;
}

FSimpleAtomic_Runnable::~FSimpleAtomic_Runnable()
{
}

bool FSimpleAtomic_Runnable::Init()
{
	return true;
}

uint32 FSimpleAtomic_Runnable::Run()
{
	//FPlatformProcess::Sleep(1.0f); use sleep for debug
	for (int i = 0; i < NumberIteration; ++i)
	{
		if (SeparateLogicFlag)
		{
			if (bIsUseAtomicFlag)
			{
				GameMode_Ref->AtomicCounter1.fetch_add(1);
				FPlatformAtomics::InterlockedIncrement(&GameMode_Ref->NotAtomicCounter1);
			}
			else
				GameMode_Ref->NotAtomicCounter1++;
		}
		else
		{
			if (bIsUseAtomicFlag)
			{
				GameMode_Ref->AtomicCounter2.fetch_add(1);
				FPlatformAtomics::InterlockedIncrement(&GameMode_Ref->NotAtomicCounter2);
			}
			else
				GameMode_Ref->NotAtomicCounter2++;
		}				
	}
	
	return 0;
}

void FSimpleAtomic_Runnable::Stop()
{
}

void FSimpleAtomic_Runnable::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("FNameGeneratorLogic_Runnable::Exit "));
}
