// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleCollectable_Runnable.h"
//need for build receiver
#include "MessageEndpointBuilder.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"

FSimpleCollectable_Runnable::FSimpleCollectable_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor)
{
	GameMode_Ref = OwnerActor;

	//IMessageBus sender init
	SenderEndpoint = FMessageEndpoint::Builder("Sender_FSimpleCollectable_Runnable").Build();
}

FSimpleCollectable_Runnable::~FSimpleCollectable_Runnable()
{
}

uint32 FSimpleCollectable_Runnable::Run()
{
	uint32 i = 0;
	while (!bIsStopCollectable)
	{
		FPlatformProcess::Sleep(2.0f);
		
		FInfoNPC newNPC;
		newNPC.Id = i;
		i++;
		
		int32 SizeNames = GameMode_Ref->FirstNames.Num();
		if (SizeNames > 0)
		{
			int32 RandNameIndex = FMath::RandHelper(SizeNames-1);

			GameMode_Ref->FirstNameMutex.Lock();
			newNPC.Name = GameMode_Ref->FirstNames[RandNameIndex];
			GameMode_Ref->FirstNames.RemoveAt(RandNameIndex);	
			GameMode_Ref->FirstNameMutex.Unlock();	
		}

		TArray<FString> AviableSecondNames = GameMode_Ref->GetSecondNames();

		int32 SizeSecondNames = AviableSecondNames.Num();
		if (SizeNames > 0)
		{
			int32 RandNameIndex = FMath::RandHelper(SizeSecondNames-1);
			//available place for critical section or change logic with TQUEUE
			newNPC.SecondName = AviableSecondNames[RandNameIndex];
			GameMode_Ref->CurrentSecondName.Remove(newNPC.SecondName);			
		}

		{
			FScopeLock NpcScopedLock(&GameMode_Ref->NPCInfoMutex);
			GameMode_Ref->NPCInfo.Add(newNPC);	
		}
		
		//IMessageBus send
		if(SenderEndpoint.IsValid())
			SenderEndpoint->Publish<FInfoNPC>(new FInfoNPC(newNPC));
		
		//how spawn? check AThreadExampleGameModeBase::EventMessageNPC
		//myCuteCube = Cast<ADumbCuteCube>(myWorld->SpawnActor(GameMode_Ref->SpawnObjectThread); //we can't create AActor object UObject in Separate Thread only in InGameThread, not recommender and not safe
	}
	return 0;
}

void FSimpleCollectable_Runnable::Stop()
{
	bIsStopCollectable = true;	
}

void FSimpleCollectable_Runnable::Exit()
{
	if(SenderEndpoint.IsValid())
		SenderEndpoint.Reset();
	
	GameMode_Ref = nullptr;
}
