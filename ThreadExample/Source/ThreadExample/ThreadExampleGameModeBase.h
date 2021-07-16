// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

//need to
#include "HAL/RunnableThread.h"

//custom 


#include "ThreadExample/SynchronizationPrimitives/SimpleCounter_Runnable.h"
#include "ThreadExample/SynchronizationPrimitives/FSimpleAtomic_Runnable.h"



#include "ThreadExampleGameModeBase.generated.h"
USTRUCT(BlueprintType, Atomic)
struct FInfoNPC
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Id = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = "none";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SecondName = "none";	
};

struct my_struct
{
	int32 Id;
	char Name;
	//std::string SecondName; //we 'can't use because requires string must to be trivially copyable, copy constructible, move constructible, copy assignable, and move assignable.
};


/**
 * 
 */
UCLASS()
class THREADEXAMPLE_API AThreadExampleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FString GlobalString = "None";
	UPROPERTY(BlueprintReadWrite)
	int32 GlobalValue = 0;

	void SetGlobalValue(int32 newValue){GlobalValue = newValue;}
	void SetGlobalString(FString newString){GlobalString = newString;}

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void PrintMessage(FString message);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreadExampleGameModeBase setting")
	bool ShowDebugAllThread = false;

	//SimpleCounter Setting
	UPROPERTY(BlueprintReadWrite, Category = "SimpleCounter setting")
	bool bIsUseSafeVariable = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleCounter setting")
	FColor ColorThread;
	UPROPERTY(BlueprintReadWrite, Category = "SimpleCounter setting")
	bool bIsUseFEvent = true;
	UPROPERTY(BlueprintReadWrite, Category = "SimpleCounter setting")
	bool bIsUseFSScopedEvent = true;

	class FSimpleCounter_Runnable *MyRunnableClass_SimpleCounter = nullptr;
	FRunnableThread* CurrentRunningGameModeThread_SimpleCounter = nullptr;
	FEvent *SimpleCounterEvent;
	
	
	//SimpleCounter Control
	UFUNCTION(BlueprintCallable)
	void StopSimpleCounterThread();
	UFUNCTION(BlueprintCallable)
	void KillSimpleCounterThread(bool bIsShouldWait);
	UFUNCTION(BlueprintCallable)
	bool SwitchRunStateSimpleCounterThread(bool bIsPause);
	UFUNCTION(BlueprintCallable)
	void CreateSimpleCounterThread();
	UFUNCTION(BlueprintCallable)
	void StartSimpleCounterThreadWithEvent();
	UFUNCTION(BlueprintCallable)
	void StartSimpleCounterThreadWithScopedEvent();
	UFUNCTION(BlueprintCallable)
	int64 GetCounterSimpleCounterThread();

	void SendRef_ScopedEvent(FScopedEvent &ScopedEvent_Ref);
	FScopedEvent* SimpleCounterScopedEvent_Ref;
	
	//SimpleAtomic Setting
	TArray<FRunnableThread*> CurrentRunningGameModeThread_SimpleRandomize;
	UPROPERTY(BlueprintReadWrite, Category = "SimpleAtomic setting")
	int32 IterationForRunnableCircle = 100000;
	UPROPERTY(BlueprintReadWrite, Category = "SimpleAtomic setting")
	int32 NumberOfThreadToCreate = 12;
	UPROPERTY(BlueprintReadWrite, Category = "SimpleAtomic setting")
	bool bIsUseAtomic = true;
	//SimpleAtomic Control
	UFUNCTION(BlueprintCallable)
	void CreateSimpleAtomicThread();
	UFUNCTION(BlueprintCallable)
	void GetCounterSimpleAtomic(int32 &Atomic1, int32 &NotAtomic1, int32 &Atomic2, int32 &NotAtomic2);
	UFUNCTION(BlueprintCallable)
	void ResetCounterSimpleAtomic();
	//SimpleAtomic storage
	std::atomic_uint16_t AtomicCounter1;
	std::atomic_uint16_t AtomicCounter2;
	int16 NotAtomicCounter1;
	int16 NotAtomicCounter2;

	//SimpleMutex Setting
	//SimpleMutex Control
	//SimpleMutex storage
	TQueue<FName,EQueueMode::Spsc> FirstNames;
	TQueue<FName,EQueueMode::Spsc> SecondNames;
};
