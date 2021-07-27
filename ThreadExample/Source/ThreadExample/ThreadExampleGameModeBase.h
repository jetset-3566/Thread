// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

//need to
#include "HAL/RunnableThread.h"

//custom 
#include "IMessageBus.h"

#include "ThreadExample/SynchronizationPrimitives/SimpleAtomic_Runnable.h"
#include "ThreadExample/SynchronizationPrimitives/SimpleCounter_Runnable.h"
#include "ThreadExample/SynchronizationPrimitives/SimpleMutex_Runnable.h"
#include "ThreadExample/SynchronizationPrimitives/SimpleCollectable_Runnable.h"

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

USTRUCT()
struct FBusStructMessage
{
	GENERATED_BODY()

	
	bool bIsSecondName = false;
	FString TextName = "None";
	FBusStructMessage(bool InBool = false, FString InText = "None") : bIsSecondName(InBool), TextName(InText) {}
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateByThread, bool, bIsSecond, FString, StringData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateByThreadNPC, FInfoNPC, NPCData);

/**
 * All setting and control thread BP-CPP here
 * SimpleCounter - how control thread and rule thread destroy with FThreadSafe variable (pause, kill, stop) and with semaphore on FEvent and FScopedEvent
 * SimpleAtomic - how save read write in thread with atomic - FPlatformAtomic and std::atomic
 * SimpleMutex - how save read write in thread with Locking - FCriticalSection and FSpinLocks and FScopeLock
 */
UCLASS()
class THREADEXAMPLE_API AThreadExampleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnUpdateByThread OnUpdateByThread;
	UPROPERTY(BlueprintAssignable)
	FOnUpdateByThreadNPC OnUpdateByThreadNPC;
	

	// Message handler for FJumpNowMessage, called by the Message Bus when a message arrives
	void BusMessageHandler(const struct FBusStructMessage& Message,
	                    const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void BusMessageHandlerNPCInfo(const struct FInfoNPC& Message,
						const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> ReceiveEndpoint;
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> ReceiveEndpointNPCInfo;
	
	
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
	FScopedEvent* SimpleCounterScopedEvent_Ref;
	
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
	
	
	//SimpleAtomic Setting
	TArray<FRunnableThread*> CurrentRunningGameModeThread_SimpleAtomic;
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
	//std::atomic<my_struct> my_Struct; we can set atomic struct and more, (warning) write and read must be full on all variables)

	//SimpleMutex Setting
	TArray<FRunnableThread*> CurrentRunningGameModeThread_SimpleMutex;
	FRunnableThread* CurrentRunningGameModeThread_SimpleCollectable;
	
	//SimpleMutex Control
	UFUNCTION(BlueprintCallable)
	void StopSimpleMutexThreads();
	UFUNCTION(BlueprintCallable)
	void CreateSimpleMutexThread();
	UFUNCTION(BlueprintCallable)
	void CreateCollectableThread();
	TArray<FInfoNPC> GetNPCInfo();
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetSecondNames();
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetFirstNames();
	UFUNCTION(BlueprintCallable)
	void Clear();

	void EventMessage(bool bIsSecond, FString StringData);
	void EventMessageNPC(FInfoNPC NPCData);
	//SimpleMutex storage
	TArray<FString> FirstNames;
	
	FCriticalSection FirstNameMutex;
	TQueue<FString,EQueueMode::Mpsc> SecondNames;

	FCriticalSection NPCInfoMutex;
	TArray<FInfoNPC> NPCInfo;

	TArray<FString> CurrentSecondName;
	//TLockFreePointerList //Same TQueue (TQueue under the hood linked list(LockFreePointerList))
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMutex setting")
	TSubclassOf<class ADumbCuteCube> SpawnObjectThread;
	int32 cubeCout =0;

	//ParallelFor
	UFUNCTION(BlueprintCallable)
	void StartParallelFor();
	UPROPERTY(BlueprintReadWrite)
	int32 ParrallelCout = 0;
};







