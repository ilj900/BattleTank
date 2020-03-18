// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!PlayerTank)
		return;
	auto ControlledTank = Cast<ATank>(GetPawn());
	if (!ControlledTank)
		return;

	MoveToActor(PlayerTank, AcceptanceRadius);
	ControlledTank->AimAt(PlayerTank->GetActorLocation());
	ControlledTank->Fire();
}