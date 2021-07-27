// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThreadExampleGameModeBase.h"
#include "DumbCuteCube.generated.h"

UCLASS(BlueprintType)
class THREADEXAMPLE_API ADumbCuteCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADumbCuteCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(FInfoNPC InitInfo);

	UFUNCTION(BlueprintNativeEvent)
	void InitBP(int32 cout, const FString& Name, const FString& SecondName);
};