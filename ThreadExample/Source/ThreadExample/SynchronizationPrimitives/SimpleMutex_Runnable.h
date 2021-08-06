// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageEndpoint.h"


class AThreadExampleGameModeBase;
/**
* 
*/
class THREADEXAMPLE_API FSimpleMutex_Runnable : public FRunnable
{
	public:
	FSimpleMutex_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor, bool bIsSecondMode);
	virtual ~FSimpleMutex_Runnable() override;

	//setting
	FColor ThreadColor;
	AThreadExampleGameModeBase *GameMode_Ref = nullptr;
	bool bIsGenerateSecondName = false;
	FThreadSafeBool bIsStopNameGenerator = false;
	
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	//random generators
	int8 GetRandom(int8 min, int8 max);
	bool GetRandom();

	//IMessageBus Setting
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> SenderEndpoint;
};

