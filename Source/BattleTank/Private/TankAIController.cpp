// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAIController.h"

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto Tank = GetControlledTank();
	if (Tank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller posessing: %s"), *(Tank->GetName()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI not posesing a tank"));
	}

	auto PC = GetPlayerTank();
	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller found Player Controller: %s"), *(PC->GetName()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI failed to find Player Controller"));
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetPlayerTank())
	{
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
	}
}

ATank* ATankAIController::GetPlayerTank() const
{
	auto PC = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PC)
		return Cast<ATank>(PC);
	return nullptr;
}