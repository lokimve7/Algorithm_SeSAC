// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"
#include <Components/Border.h>
#include <Components/TextBlock.h>
#include <Components/WidgetComponent.h>

// Sets default values
ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();
	
	UWidgetComponent* widget = Cast<UWidgetComponent>(GetComponentByClass(UWidgetComponent::StaticClass()));
	
	outLine = Cast<UBorder>(widget->GetUserWidgetObject()->GetWidgetFromName(TEXT("OutLine")));
	textTotal = Cast<UTextBlock>(widget->GetUserWidgetObject()->GetWidgetFromName(TEXT("Total")));
	textStart = Cast<UTextBlock>(widget->GetUserWidgetObject()->GetWidgetFromName(TEXT("byStart")));
	textEnd = Cast<UTextBlock>(widget->GetUserWidgetObject()->GetWidgetFromName(TEXT("byEnd")));
}

// Called every frame
void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACube::SetCost(ACube* currCube, ACube* goalCube)
{
	FVector vCurr = currCube->GetActorLocation();
	FVector vGoal = goalCube->GetActorLocation();
	FVector vMy = GetActorLocation();
	
	// ������ ���� ���
	startCost = currCube->startCost + FMath::Abs(vCurr.X - vMy.X) + FMath::Abs(vCurr.Y - vMy.Y);
	// ������ ���� ���
	endCost = FMath::Abs(vGoal.X - vMy.X) + FMath::Abs(vGoal.Y - vMy.Y);
	// ���� ���
	totalCost = startCost + endCost;

	textTotal->SetText(FText::AsNumber((int32)totalCost));
	textStart->SetText(FText::AsNumber((int32)startCost));
	textEnd->SetText(FText::AsNumber((int32)endCost));

	SetColor(FLinearColor::Blue);

	parent = currCube;

	UE_LOG(LogTemp, Warning, TEXT("%s --> s : %f, e : %f, t : %f"), *GetActorNameOrLabel(), startCost, endCost, totalCost);

}

void ACube::SetColor(FLinearColor color)
{
	outLine->SetBrushColor(color);
}

