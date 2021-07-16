// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/ThreadManager.h"

class AThreadExampleGameModeBase;
/**
 * 
 */
class THREADEXAMPLE_API FSimpleAtomic_Runnable : public FRunnable
{
public:
	FSimpleAtomic_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor, uint32 NeedIteration, bool SeparateLogic, bool bIsUseAtomic);
	virtual ~FSimpleAtomic_Runnable() override;


	//setting
	FColor ThreadColor;
	AThreadExampleGameModeBase *GameMode_Ref = nullptr;
	bool bIsStopThread = false;
	int NumberIteration = 0;
	bool SeparateLogicFlag;
	bool bIsUseAtomicFlag;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
};
