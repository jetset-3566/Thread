// Fill out your copyright notice in the Description page of Project Settings.


#include "DumbCuteCube.h"


// Sets default values
ADumbCuteCube::ADumbCuteCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADumbCuteCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADumbCuteCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADumbCuteCube::Init(FInfoNPC InitInfo)
{
	InitBP(InitInfo.Id, InitInfo.Name, InitInfo.SecondName);
}

void ADumbCuteCube::InitBP_Implementation(int32 cout, const FString& Name, const FString& SecondName)
{
	
}




