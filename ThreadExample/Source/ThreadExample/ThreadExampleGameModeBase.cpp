// Copyright Epic Games, Inc. All Rights Reserved.


#include "ThreadExampleGameModeBase.h"



void AThreadExampleGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	/*FString myThreadType = "None";
	uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();

	UE_LOG(LogTemp, Error, TEXT("AThreadExampleGameModeBase::Tick Id - %d  Name - %s , Priority - %s"), ThreadId, *FThreadManager::Get().GetThreadName(ThreadId), *myThreadType);*/

	if (ShowDebugAllThread)
	{
		FThreadManager::Get().ForEachThread([&](uint32 ThreadId, FRunnableThread* Runnable)
		{
			FString myThreadType = "None";
			//UEnum::GetValueAsString<EThreadPriority>(Runnable->GetThreadPriority(), myThreadType);

		/*	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EThreadPriority"), true);
		if (!EnumPtr) { NSLOCTEXT("Invalid", "Invalid", "Invalid");}
		else
		myThreadType = EnumPtr->GetEnumName(Runnable->GetThreadPriority());*/


			switch (Runnable->GetThreadPriority())
			{
			case TPri_Normal:
				myThreadType = "TPri_Normal";
				break;
			case TPri_AboveNormal:
				myThreadType = "TPri_AboveNormal";
				break;
			case TPri_BelowNormal:
				myThreadType = "TPri_BelowNormal";
				break;
			case TPri_Highest:
				myThreadType = "TPri_Highest";
				break;
			case TPri_Lowest:
				myThreadType = "TPri_Lowest";
				break;
			case TPri_SlightlyBelowNormal:
				myThreadType = "TPri_SlightlyBelowNormal";
				break;
			case TPri_TimeCritical:
				myThreadType = "TPri_TimeCritical";
				break;
			case TPri_Num:
				myThreadType = "TPri_Num";
				break;
			default:
				break;
			}

			if (Runnable->GetThreadName() == "Calc Thread")
			{
				UE_LOG(LogTemp, Warning, TEXT("AThreadExampleGameModeBase::Tick Calc Thread cathc - , Priority - %s"), *myThreadType);
			}

			UE_LOG(LogTemp, Error, TEXT("AThreadExampleGameModeBase::Tick Id - %d  Name - %s , Priority - %s"), ThreadId, *Runnable->GetThreadName(), *myThreadType);
		});
	}	
}

void AThreadExampleGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AThreadExampleGameModeBase::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StopSimpleCounterThread();
}

//----------------------------------SimpleCounter start--------------------------------
void AThreadExampleGameModeBase::PrintMessage(FString message)
{
	//UE_LOG(LogTemp, Error, TEXT("AThreadExampleGameModeBase::printMessage()"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
}

void AThreadExampleGameModeBase::StopSimpleCounterThread()
{
	if (CurrentRunningGameModeThread_SimpleCounter)
	{
		if (MyRunnableClass_SimpleCounter)
		{
			//safe stop thread
			MyRunnableClass_SimpleCounter->bIsStopThreadSafe = true;
			//Not safe stop thread
			MyRunnableClass_SimpleCounter->bIsStopThread = true;

			if(!MyRunnableClass_SimpleCounter->bIsUseSafeVariable)//for kill thread stop crush
			{
				CurrentRunningGameModeThread_SimpleCounter->Suspend(false);	
			}

			if (SimpleCounterEvent)//event close
			{
				//SimpleCounterEvent set on wait 10 second if we close game after 10 second exe closed
				FPlatformProcess::ReturnSynchEventToPool(SimpleCounterEvent);
				SimpleCounterEvent = nullptr;
			}
			if (SimpleCounterScopedEvent_Ref)
			{
				//SimpleCounterScopedEvent_Ref->Trigger(); use ReturnSynchEventToPool to Remove all guard 
				FPlatformProcess::ReturnSynchEventToPool(SimpleCounterScopedEvent_Ref->Get());
				SimpleCounterScopedEvent_Ref=nullptr;
			}

			//End logic runnable thread 
			CurrentRunningGameModeThread_SimpleCounter->WaitForCompletion();
			
			delete MyRunnableClass_SimpleCounter;
			MyRunnableClass_SimpleCounter = nullptr;
		}
	}
}

void AThreadExampleGameModeBase::KillSimpleCounterThread(bool bIsShouldWait)
{
	if (CurrentRunningGameModeThread_SimpleCounter)
	{
		if (MyRunnableClass_SimpleCounter)
		{
			CurrentRunningGameModeThread_SimpleCounter->Suspend(false);
			CurrentRunningGameModeThread_SimpleCounter->Kill(bIsShouldWait);
		}
	}
}

bool AThreadExampleGameModeBase::SwitchRunStateSimpleCounterThread(bool bIsPause)
{
	if (CurrentRunningGameModeThread_SimpleCounter && MyRunnableClass_SimpleCounter)
	{
		CurrentRunningGameModeThread_SimpleCounter->Suspend(bIsPause);		
	}
	return bIsPause;
}

void AThreadExampleGameModeBase::CreateSimpleCounterThread()
{
	if (bIsUseFEvent)
	{
		SimpleCounterEvent = FPlatformProcess::GetSynchEventFromPool();	
	}

	if (!CurrentRunningGameModeThread_SimpleCounter)
	{
		if (!MyRunnableClass_SimpleCounter)
		{
			MyRunnableClass_SimpleCounter = new FSimpleCounter_Runnable(ColorThread, this, bIsUseSafeVariable);	
		}	
		CurrentRunningGameModeThread_SimpleCounter = FRunnableThread::Create(MyRunnableClass_SimpleCounter, TEXT("SimpleCounter Thread"), 0, EThreadPriority::TPri_Normal);	
	}	
}

void AThreadExampleGameModeBase::StartSimpleCounterThreadWithEvent()
{
	if (SimpleCounterEvent)
	{
		SimpleCounterEvent->Trigger();
	}
}

void AThreadExampleGameModeBase::StartSimpleCounterThreadWithScopedEvent()
{
	if (SimpleCounterScopedEvent_Ref)
	{
		SimpleCounterScopedEvent_Ref->Trigger();
	}
}


int64 AThreadExampleGameModeBase::GetCounterSimpleCounterThread()
{
	int64 result = -1;
	if (MyRunnableClass_SimpleCounter)
	{
		if (MyRunnableClass_SimpleCounter->bIsUseSafeVariable)
		{
			result = MyRunnableClass_SimpleCounter->CounterSafe.GetValue();
			//printMessage(FString::FromInt(MyRunnableClass_NameGenerator->CounterSafe.GetValue()));
		}
		else
		{
			result = MyRunnableClass_SimpleCounter->Counter;
			//printMessage(FString::FromInt(MyRunnableClass_NameGenerator->Counter));	
		}		
	}
	return result;
}

void AThreadExampleGameModeBase::SendRef_ScopedEvent(FScopedEvent &ScopedEvent_Ref)
{
	SimpleCounterScopedEvent_Ref = &ScopedEvent_Ref;
}

//SimpleCounter end

//----------------------------------SimpleAtomic start--------------------------------
void AThreadExampleGameModeBase::CreateSimpleAtomicThread()
{
	for (int i = 0; i < NumberOfThreadToCreate; ++i)
	{
		bool bFlag = false;
		if (i%2)
			bFlag = true;
		
		class FSimpleAtomic_Runnable *MyRunnableClass_SimpleRandomize = new FSimpleAtomic_Runnable(ColorThread, this, IterationForRunnableCircle, bFlag, bIsUseAtomic);
		CurrentRunningGameModeThread_SimpleRandomize.Add(FRunnableThread::Create(MyRunnableClass_SimpleRandomize, TEXT("SimpleRandomize Thread"), 0, EThreadPriority::TPri_Normal));	
	} 	
}

void AThreadExampleGameModeBase::GetCounterSimpleAtomic(int32 &Atomic1, int32 &NotAtomic1, int32 &Atomic2, int32 &NotAtomic2)
{
	if (bIsUseAtomic)
	{
		NotAtomic1 = FPlatformAtomics::AtomicRead(&NotAtomicCounter1);
		NotAtomic2 = FPlatformAtomics::AtomicRead(&NotAtomicCounter2);
		if(AtomicCounter2.is_lock_free())
		{
			Atomic2 = AtomicCounter2;
		}
	
		if(AtomicCounter1.is_lock_free())
		{
			Atomic1 = AtomicCounter1;
		}
	}
	else
	{
		NotAtomic1 = NotAtomicCounter1;
		NotAtomic2 = NotAtomicCounter2;
	}
}

void AThreadExampleGameModeBase::ResetCounterSimpleAtomic()
{
	AtomicCounter1 = 0;
	AtomicCounter2 = 0;
	NotAtomicCounter1 = 0;
	NotAtomicCounter2 = 0;
}

//SimpleAtomic End

//----------------------------------SimpleMutex start--------------------------------
//
//SimpleMutex End
