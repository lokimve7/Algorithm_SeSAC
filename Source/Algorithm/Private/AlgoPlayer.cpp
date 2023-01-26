// Fill out your copyright notice in the Description page of Project Settings.


#include "AlgoPlayer.h"
#include "AlgoPuzzle.h"
#include <Blueprint/WidgetLayoutLibrary.h>
#include "Cube.h"

// Sets default values
AAlgoPlayer::AAlgoPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//퍼즐 클래스 가져오자
	ConstructorHelpers::FClassFinder<UAlgoPuzzle> tempPuzzle(TEXT("WidgetBlueprint'/Game/BP_AlgoPuzzle.BP_AlgoPuzzle_C'"));
	if (tempPuzzle.Succeeded())
	{
		puzzleFactory = tempPuzzle.Class;
	}
}

// Called when the game starts or when spawned
void AAlgoPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//퍼즐 화면 보이게 하자
	/*puzzle = CreateWidget<UAlgoPuzzle>(GetWorld(), puzzleFactory);
	puzzle->AddToViewport();*/

	//마우스 커서 보이게 하자
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

// Called every frame
void AAlgoPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SimpleMove();
}

void AAlgoPlayer::SimpleMove()
{
	//만약에 pathArray 에 값이 없다면 함수를 나간다.
	if (pathArray.Num() == 0) return;

	currTime += GetWorld()->GetDeltaSeconds();
	if (currTime > 0.3f)
	{
		currTime = 0;
		FVector pos = pathArray[0]->GetActorLocation();
		pos.Z = 100;
		SetActorLocation(pos);

		pathArray.RemoveAt(0);
	}
}

// Called to bind functionality to input
void AAlgoPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//마우스 왼쪽 버튼
	PlayerInputComponent->BindAction(TEXT("MouseL"), IE_Pressed, this, &AAlgoPlayer::InputMouseLDown);
	PlayerInputComponent->BindAction(TEXT("MouseL"), IE_Released, this, &AAlgoPlayer::InputMouseLUp);
}

void AAlgoPlayer::InputMouseLDown()
{
	if (startCube != nullptr)
	{
		FindPath();
		return;
	}

	//초기셋팅
	for (int32 i = 0; i < openArray.Num(); i++)
	{
		openArray[i]->SetInit();
	}
	for (int32 i = 0; i < closeArray.Num(); i++)
	{
		closeArray[i]->SetInit();
	}
	openArray.Empty();
	closeArray.Empty();


	//마우스 클릭 좌표를 얻어오자
	FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	
	//선택된 카드 알아오자
	//puzzle->SelectCard(mousePos);

	FVector start, end;
	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	//start Cube 알아오자
	start = GetActorLocation() + FVector::UpVector;
	end = start + FVector::DownVector * 2;
	bool result = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, params);
	if (result == true)
	{
		startCube = Cast<ACube>(hit.GetActor());
		UE_LOG(LogTemp, Warning, TEXT("start : %s"), *hit.GetActor()->GetActorNameOrLabel());

		openArray.Add(startCube);

	}

	//goal Cube 알아오자
	FVector dir;
	//마우스 클릭위치를 3D 공간의 좌표로 바꾸자
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(start, dir);
	end = start + dir * 100000;
	result = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, params);
	if (result == true)
	{
		goalCube = Cast<ACube>(hit.GetActor());
		UE_LOG(LogTemp, Warning, TEXT("goal : %s"), *hit.GetActor()->GetActorNameOrLabel());
	}

	FindPath();
}

void AAlgoPlayer::InputMouseLUp()
{
	//카드 초기화
	//puzzle->InitCard();
}

void AAlgoPlayer::FindPath()
{
	//기준이 되는 큐브 설정
	currCube = openArray[0];

	//오른쪽
	AddOpen(FVector::RightVector);
	//왼쪽
	AddOpen(FVector::LeftVector);
	//뒤
	AddOpen(FVector::BackwardVector);
	//앞
	AddOpen(FVector::ForwardVector);

	//currCube 를 openArray에 뺀다
	openArray.Remove(currCube);
	//currCube 를 closeArray에 추가
	closeArray.Add(currCube);
	//currCube 의 테두리 색을 빨간색으로 바꾼다.
	currCube->SetColor(FLinearColor::Red);

	//Goal 까지 찾아갔느냐? (길찾기가 끝났니?)
	if (openArray[0] == goalCube || openArray.Num() == 0)
	{
		//최단거리를 노란색으로 표시하자
		ACube* cube = goalCube;
		//부모 Cube 가 없을때까지 반복
		while (cube->parentCube != nullptr)
		{
			//진짜 길 추가
			pathArray.Insert(cube, 0);

			cube->SetColor(FLinearColor::Yellow);
			cube = cube->parentCube;
		}
		//처음 큐브 추가
		pathArray.Insert(startCube, 0);
		startCube->SetColor(FLinearColor::Yellow);

		//startCube, goalCube 초기화
		startCube = nullptr;
		goalCube = nullptr;
	}
	else
	{
		FindPath();
	}
}

void AAlgoPlayer::FindNear()
{

}

void AAlgoPlayer::AddOpen(FVector dir)
{
	FHitResult hit;
	FVector start, end;
	FCollisionQueryParams param;
	param.AddIgnoredActor(currCube);

	start = currCube->GetActorLocation();
	end = start + dir * 100;
	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, param))
	{
		ACube* cube = Cast<ACube>(hit.GetActor());
		//openArray, closeArray 에 존재 하지 않는다면
		if (openArray.Contains(cube) == false && closeArray.Contains(cube) == false)
		{
			//부딪힌 큐브의 Cost(비용) 구하자
			cube->SetCost(currCube, goalCube);

			//openArray 에 들어있는 큐브들의 totalCost 와 cube 의 totalCost 를 비교
			int32 i = 0;
			for (i = 0; i < openArray.Num(); i++)
			{
				if (openArray[i]->totalCost >= cube->totalCost)
				{
					break;
				}
			}

			//openArray 추가
			openArray.Insert(cube, i);
		}
	}
}