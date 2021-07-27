// Copyright Epic Games, Inc. All Rights Reserved.


#include "ThreadExampleGameModeBase.h"

#include "DumbCuteCube.h"
#include "MessageEndpointBuilder.h"
#include "Misc/ScopeTryLock.h"

void AThreadExampleGameModeBase::BusMessageHandler(const FBusStructMessage& Message,
                                                const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	EventMessage(Message.bIsSecondName, Message.TextName);
}

void AThreadExampleGameModeBase::BusMessageHandlerNPCInfo(const FInfoNPC& Message,
	const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	EventMessageNPC(Message);
}

void AThreadExampleGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (ShowDebugAllThread)
	{
		FThreadManager::Get().ForEachThread([&](uint32 ThreadId, FRunnableThread* Runnable)
		{
			FString myThreadType = "None";
			
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

	ReceiveEndpoint = FMessageEndpoint::Builder("Resiever_AThreadExampleGameModeBase")
										.Handling<FBusStructMessage>(this, &AThreadExampleGameModeBase::BusMessageHandler);

	if(ReceiveEndpoint.IsValid())
		ReceiveEndpoint->Subscribe<FBusStructMessage>();
	
	ReceiveEndpointNPCInfo = FMessageEndpoint::Builder("Resiever_NPC_AThreadExampleGameModeBase")
									.Handling<FInfoNPC>(this, &AThreadExampleGameModeBase::BusMessageHandlerNPCInfo);

	if(ReceiveEndpointNPCInfo.IsValid())
		ReceiveEndpointNPCInfo->Subscribe<FInfoNPC>();
}

void AThreadExampleGameModeBase::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StopSimpleCounterThread();
	StopSimpleMutexThreads();
	
	if(ReceiveEndpoint.IsValid())
		ReceiveEndpoint.Reset();
	if (ReceiveEndpointNPCInfo.IsValid())
		ReceiveEndpointNPCInfo.Reset();
}

void AThreadExampleGameModeBase::PrintMessage(FString message)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
}

//----------------------------------SimpleCounter start--------------------------------
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

			bIsUseFSScopedEvent = false; // try to not create ScopedLock when exit app, if scopedEvent go after event
			
			//if(!MyRunnableClass_SimpleCounter->bIsUseSafeVariable)//for stop thread, stop crush - if thread was in pause state
			//{
				CurrentRunningGameModeThread_SimpleCounter->Suspend(false);	
			//}

			if (SimpleCounterEvent)//event close
			{				
				SimpleCounterEvent->Trigger();//if thread of app wait, trigger for exit
				//SimpleCounterEvent set on wait 10 second if we close game after 10 second exe closed
				FPlatformProcess::ReturnSynchEventToPool(SimpleCounterEvent);//best practice for Event
				SimpleCounterEvent = nullptr;//for not wait second wait if we want exit 
			}
			if (SimpleCounterScopedEvent_Ref)
			{
				SimpleCounterScopedEvent_Ref->Trigger();//if thread of app wait, trigger for exit
				
				//FPlatformProcess::ReturnSynchEventToPool(SimpleCounterScopedEvent_Ref->Get());
				SimpleCounterScopedEvent_Ref = nullptr;
			}

			//End logic runnable thread 
			CurrentRunningGameModeThread_SimpleCounter->WaitForCompletion();

			if (MyRunnableClass_SimpleCounter)
			{
				//delete MyRunnableClass_SimpleCounter; // not need call destructor GC do it
				MyRunnableClass_SimpleCounter = nullptr;
			}
			 
			if (CurrentRunningGameModeThread_SimpleCounter)
			{
				//delete CurrentRunningGameModeThread_SimpleCounter; // not need call destructor GC do it
				CurrentRunningGameModeThread_SimpleCounter = nullptr;	
			}
			
		}
	}
}

void AThreadExampleGameModeBase::KillSimpleCounterThread(bool bIsShouldWait)
{
	if (CurrentRunningGameModeThread_SimpleCounter)
	{
		//if (MyRunnableClass_SimpleCounter) not needed
		CurrentRunningGameModeThread_SimpleCounter->Suspend(false);
		CurrentRunningGameModeThread_SimpleCounter->Kill(bIsShouldWait);
		CurrentRunningGameModeThread_SimpleCounter = nullptr;
		MyRunnableClass_SimpleCounter = nullptr;
	}
}

bool AThreadExampleGameModeBase::SwitchRunStateSimpleCounterThread(bool bIsPause)
{
	if (CurrentRunningGameModeThread_SimpleCounter)
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
		CurrentRunningGameModeThread_SimpleCounter = FRunnableThread::Create(MyRunnableClass_SimpleCounter, TEXT("SimpleCounter Thread1"), 0, EThreadPriority::TPri_Normal);

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
		SimpleCounterScopedEvent_Ref = nullptr;
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
		
		class FSimpleAtomic_Runnable *MyRunnableClass_SimpleAtomic = new FSimpleAtomic_Runnable(ColorThread, this, IterationForRunnableCircle, bFlag, bIsUseAtomic);
		CurrentRunningGameModeThread_SimpleAtomic.Add(FRunnableThread::Create(MyRunnableClass_SimpleAtomic, TEXT("SimpleRandomize Thread"), 0, EThreadPriority::TPri_Normal));	
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
void AThreadExampleGameModeBase::StopSimpleMutexThreads()
{
	if (CurrentRunningGameModeThread_SimpleMutex.Num() > 0)
	{
		for (auto RunnableThread : CurrentRunningGameModeThread_SimpleMutex)
		{
			if (RunnableThread)
			{
				RunnableThread->Kill(true);	
			}						
		}
		CurrentRunningGameModeThread_SimpleMutex.Empty();
	}
	if (CurrentRunningGameModeThread_SimpleCollectable)
	{
		CurrentRunningGameModeThread_SimpleCollectable->Kill(true);
		CurrentRunningGameModeThread_SimpleCollectable = nullptr;
	}
}

void AThreadExampleGameModeBase::CreateSimpleMutexThread()
{
	for (int i = 0; i < 4; ++i)
	{
		bool bFlag = false;
		if (i%2)
			bFlag = true;
		
		class FSimpleMutex_Runnable *MyRunnableClass_SimpleMutex = new FSimpleMutex_Runnable(ColorThread, this, bFlag);
		CurrentRunningGameModeThread_SimpleMutex.Add(FRunnableThread::Create(MyRunnableClass_SimpleMutex, TEXT("SimpleMutex Thread"), 0, EThreadPriority::TPri_Normal));	
	}	 	
}

void AThreadExampleGameModeBase::CreateCollectableThread()
{
	class FSimpleCollectable_Runnable *MyRunnableClass_SimpleCollectable = new FSimpleCollectable_Runnable(ColorThread, this);
	CurrentRunningGameModeThread_SimpleCollectable = FRunnableThread::Create(MyRunnableClass_SimpleCollectable, TEXT("SimpleMutex Thread"), 0, EThreadPriority::TPri_Normal);
}

TArray<FInfoNPC> AThreadExampleGameModeBase::GetNPCInfo()
{
	TArray<FInfoNPC> Result;
	return Result;
}

TArray<FString> AThreadExampleGameModeBase::GetSecondNames()
{
	TArray<FString> Result;
	FString OneRead;
	while (SecondNames.Dequeue(OneRead))
	{
		Result.Add(OneRead);
	}
	CurrentSecondName.Append(Result);
	return CurrentSecondName;
}

TArray<FString> AThreadExampleGameModeBase::GetFirstNames()
{
	{
		FScopeLock myScopeLock(&FirstNameMutex);
	
		return  FirstNames;		
	}		
}

void AThreadExampleGameModeBase::Clear()
{
	FirstNames.Empty();
	SecondNames.Empty();
	CurrentSecondName.Empty();
}

void AThreadExampleGameModeBase::EventMessage(bool bIsSecond, FString StringData)
{
	OnUpdateByThread.Broadcast(bIsSecond, StringData);
}

void AThreadExampleGameModeBase::EventMessageNPC(FInfoNPC NPCData)
{
	OnUpdateByThreadNPC.Broadcast(NPCData);

	UWorld *myWorld = GetWorld();
	if (myWorld)
	{
		FVector SpawnLoc = FVector(1200.0f, 100.0f * cubeCout, 500.0f);
		FRotator SpawnRot;
		ADumbCuteCube* myCuteCube;
		myCuteCube = Cast<ADumbCuteCube>(myWorld->SpawnActor(SpawnObjectThread, &SpawnLoc, &SpawnRot, FActorSpawnParameters()));
		if (myCuteCube)
		{
			myCuteCube->Init(NPCData);
			cubeCout++;
		}		
	}
}
//SimpleMutex End

//ParallelFor
void AThreadExampleGameModeBase::StartParallelFor()
{
	ParallelFor( 500, [&](int32 index)// лямбда-выражение дает в качестве параметра индекс элемента, которым можно управлять
	{		
		int32 cout = 0;
		for (int i = 0; i < 1000000; ++i)
		{
			cout++;
		}
		//can add mutex
		ParrallelCout += cout;
	} );
}




