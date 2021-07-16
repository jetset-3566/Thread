// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FRunnableThread;
class ARunnableExample;
/**
 * 
 */
class THREADEXAMPLE_API FRunnableThreadExample : public FRunnable
{
public:
	FRunnableThreadExample(int32 CalcInput, ARunnableExample *OwnerActor);
	~FRunnableThreadExample();

	//FCriticalSection myCriticalSection;//mutex

	bool bIsStopThread;

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit();

private:

	int32 Calculation;
	int32 CalcCount;

	ARunnableExample *CurrentThreadActor;
	int32 CurrentCalculation;
};
