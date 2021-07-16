// Fill out your copyright notice in the Description page of Project Settings.


#include "FSimpleMutex_Runnable.h"
#include "ThreadExample/ThreadExampleGameModeBase.h"

FSimpleMutex_Runnable::FSimpleMutex_Runnable(FColor Color, AThreadExampleGameModeBase* OwnerActor)
{
	ThreadColor = Color;
	GameMode_Ref = OwnerActor;
}

FSimpleMutex_Runnable::~FSimpleMutex_Runnable()
{
}

uint32 FSimpleMutex_Runnable::Run()
{
	return 0;
}
