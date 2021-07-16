// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HAL/RunnableThread.h"
#include "HAL/ThreadManager.h"

class AThreadExampleGameModeBase;
/**
* 
*/
class THREADEXAMPLE_API FSimpleMutex_Runnable : public FRunnable
{
	public:
	FSimpleMutex_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor);
	virtual ~FSimpleMutex_Runnable() override;

	//setting
	FColor ThreadColor;
	AThreadExampleGameModeBase *GameMode_Ref = nullptr;
	
	//virtual bool Init() override;
	virtual uint32 Run() override;
	//virtual void Stop() override;
	//virtual void Exit() override;

	bool SeparateLogicFlag;
	bool bIsUseAtomicFlag;
};
