// Fill out your copyright notice in the Description page of Project Settings.


#include "AlgoPlayer.h"
#include "AlgoPuzzle.h"
#include <Blueprint/WidgetLayoutLibrary.h>

// Sets default values
AAlgoPlayer::AAlgoPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���� Ŭ���� ��������
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
	
	//���� ȭ�� ���̰� ����
	puzzle = CreateWidget<UAlgoPuzzle>(GetWorld(), puzzleFactory);
	puzzle->AddToViewport();

	//���콺 Ŀ�� ���̰� ����
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

// Called every frame
void AAlgoPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAlgoPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//���콺 ���� ��ư
	PlayerInputComponent->BindAction(TEXT("MouseL"), IE_Pressed, this, &AAlgoPlayer::InputMouseLDown);
	PlayerInputComponent->BindAction(TEXT("MouseL"), IE_Released, this, &AAlgoPlayer::InputMouseLUp);
}

void AAlgoPlayer::InputMouseLDown()
{
	//���콺 Ŭ�� ��ǥ�� ������
	FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	
	//���õ� ī�� �˾ƿ���
	puzzle->SelectCard(mousePos);
}

void AAlgoPlayer::InputMouseLUp()
{
	//ī�� �ʱ�ȭ
	puzzle->InitCard();
}
