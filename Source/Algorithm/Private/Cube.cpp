// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"
#include <Components/WidgetComponent.h>
#include <Components/Border.h>
#include <Components/TextBlock.h>

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
	
	//Widget Component 찾아오자
	UWidgetComponent* widget = Cast<UWidgetComponent>(GetComponentByClass(UWidgetComponent::StaticClass()));
	UUserWidget* userWidget = widget->GetUserWidgetObject();

	//테투리 widget
	outLine = Cast<UBorder>(userWidget->GetWidgetFromName(TEXT("OutLine")));
	//totalCost widget
	textTotal = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("Total")));
	//startCost widget
	textStart = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("byStart")));
	//goalCost widget
	textGoal = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("byGoal")));
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
	
	// 시작점 기준 비용
	startCost = currCube->startCost + FMath::Abs(vCurr.X - vMy.X) + FMath::Abs(vCurr.Y - vMy.Y);
	// 도작점 기준 비용
	goalCost = FMath::Abs(vGoal.X - vMy.X) + FMath::Abs(vGoal.Y - vMy.Y);
	// 최종 비용
	totalCost = startCost + goalCost;

	// UI 로 표현
	textTotal->SetText(FText::AsNumber(totalCost));
	textStart->SetText(FText::AsNumber(startCost));
	textGoal->SetText(FText::AsNumber(goalCost));

	//테두리를 파란색으로 바꾸자
	SetColor(FLinearColor::Blue);

	//누구를 기준으로 Cost 를 계산했나?(부모)
	parentCube = currCube;

	UE_LOG(LogTemp, Warning, TEXT("%s --> s : %f, e : %f, t : %f"), *GetActorNameOrLabel(), startCost, goalCost, totalCost);
}

void ACube::SetColor(FLinearColor color)
{
	outLine->SetBrushColor(color);
}

void ACube::SetInit()
{
	SetColor(FLinearColor::Black);
	parentCube = nullptr;
	totalCost = startCost = goalCost = 0;

	textTotal->SetText(FText::AsNumber(totalCost));
	textStart->SetText(FText::AsNumber(startCost));
	textGoal->SetText(FText::AsNumber(goalCost));
}