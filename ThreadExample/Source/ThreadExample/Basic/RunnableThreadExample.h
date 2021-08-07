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

	FThreadSafeBool bIsStopThread = false;

	virtual bool Init() override;
	virtual uint32 Run() override;

private:

	int32 Calculation;
	int32 CurrentCalculation;
	
	ARunnableExample *CurrentThreadActor;
	
};
