// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HAL/RunnableThread.h"
#include "HAL/ThreadManager.h"

class AThreadExampleGameModeBase;
/**
 * 
 */
class THREADEXAMPLE_API FSimpleCounter_Runnable : public FRunnable
{
public:
	FSimpleCounter_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor, bool VariableMode);
	virtual ~FSimpleCounter_Runnable() override;

	//Safe
	FThreadSafeBool bIsStopThreadSafe = FThreadSafeBool(false);
	FThreadSafeCounter CounterSafe = FThreadSafeCounter(0);

	//Not Safe
	bool bIsStopThread = false;
	int Counter = 0;

	//setting
	bool bIsUseSafeVariable = false;
	FColor ThreadColor;
	AThreadExampleGameModeBase *GameMode_Ref = nullptr;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
};
