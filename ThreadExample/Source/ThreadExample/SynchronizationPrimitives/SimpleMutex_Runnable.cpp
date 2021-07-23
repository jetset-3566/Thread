// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleMutex_Runnable.h"

#include <random>

#include "MessageEndpointBuilder.h"
#include "Misc/ScopeTryLock.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"



FSimpleMutex_Runnable::FSimpleMutex_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor, bool bIsSecondMode)
{
	ThreadColor = Color;
	GameMode_Ref = OwnerActor;
	bIsGenerateSecondName = bIsSecondMode;

	//IMessageBus sender init
	SenderEndpoint = FMessageEndpoint::Builder("Sender_FSimpleMutex_Runnable").Build();

}

FSimpleMutex_Runnable::~FSimpleMutex_Runnable()
{
}

uint32 FSimpleMutex_Runnable::Run()
{
	TArray<FString> VowelLetter{"a", "e", "i", "o", "u", "y"};
	TArray<FString> ConsonantLetter{"b", "c", "d", "f", "g", "h", "j", "k", "l", "m", "n", "p", "q", "r", "s", "t", "v", "w", "x", "z"};

	while (!bIsStopNameGenerator)
	{
		FString Result;

		bool bIsStartWithVowel = GetRandom();

		int8 RandomSizeName;
		if (bIsGenerateSecondName)
		{
			RandomSizeName = GetRandom(5, 8);
		}
		else
		{
			RandomSizeName = GetRandom(3, 5);
		}


		for (int8 j = 0; j < RandomSizeName; j++)
		{
			bool FlipFlop = false;
			if (j % 2)
				FlipFlop = true;

			if (bIsStartWithVowel)
			{
				if (FlipFlop)
					Result.Append(VowelLetter[GetRandom(0, VowelLetter.Num() - 1)]);
				else
					Result.Append(ConsonantLetter[GetRandom(0, ConsonantLetter.Num() - 1)]);
			}
			else
			{
				if (FlipFlop)
					Result.Append(ConsonantLetter[GetRandom(0, ConsonantLetter.Num() - 1)]);
				else
					Result.Append(VowelLetter[GetRandom(0, VowelLetter.Num() - 1)]);
			}
		}

		FPlatformProcess::Sleep(1.88f);
		if (bIsGenerateSecondName)
		{
			GameMode_Ref->FirstNameMutex.Lock();

			GameMode_Ref->FirstNames.Add(Result);
			//FPlatformProcess::Sleep(0.02f);
			GameMode_Ref->FirstNameMutex.Unlock();
			
		}
		else
		{
			GameMode_Ref->SecondNames.Enqueue(Result);
		}

		//IMessageBus sender sending
		/*FBusStructMessage* BusStructMessage = new FBusStructMessage;
		BusStructMessage->TextName = Result;
		BusStructMessage->bIsSecondName = bIsGenerateSecondName;*/
		if(SenderEndpoint.IsValid())
			SenderEndpoint->Publish<FBusStructMessage>( new FBusStructMessage(bIsGenerateSecondName,Result));														
		//GameMode_Ref->Delegate - standard delegate ue4 not working near threads

		/*delete BusStructMessage;
		BusStructMessage = nullptr;*/
	}
	
	return 0;
}

void FSimpleMutex_Runnable::Stop()
{
	bIsStopNameGenerator = true;
}

void FSimpleMutex_Runnable::Exit()
{
	if(SenderEndpoint.IsValid())
		SenderEndpoint->Subscribe<FBusStructMessage>();
	GameMode_Ref = nullptr;
}

int8 FSimpleMutex_Runnable::GetRandom(int8 min, int8 max)
{
	std::random_device rd; 
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> distrib(min, max);

	return distrib(gen);
}

bool FSimpleMutex_Runnable::GetRandom()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::bernoulli_distribution d(0.5);

	return d(gen);
}