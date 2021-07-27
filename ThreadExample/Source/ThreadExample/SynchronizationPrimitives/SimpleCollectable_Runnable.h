// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageEndpoint.h"

class AThreadExampleGameModeBase;
/**
 * 
 */
class THREADEXAMPLE_API FSimpleCollectable_Runnable : public FRunnable
{
public:
	FSimpleCollectable_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor);
	virtual ~FSimpleCollectable_Runnable() override;

	AThreadExampleGameModeBase* GameMode_Ref = nullptr;
	FThreadSafeBool bIsStopCollectable = false;
	
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> SenderEndpoint;
};
