// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cube.generated.h"

UCLASS()
class ALGORITHM_API ACube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class UBorder* outLine;
	UPROPERTY()
	class UTextBlock* textTotal;
	UPROPERTY()
	class UTextBlock* textStart;
	UPROPERTY()
	class UTextBlock* textEnd;
	UPROPERTY()
	class ACube* parent;


	float startCost = 0;
	float endCost = 0;
	float totalCost = 0;

	void SetCost(ACube* currCube, ACube* goalCube);
	void SetColor(FLinearColor color);
};
