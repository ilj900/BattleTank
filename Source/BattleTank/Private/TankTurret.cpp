// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"

void UTankTurret::Rotate(float RelativeSpeed)
{
	auto YawChange = RelativeSpeed * MaxDegreesPerSecont * GetWorld()->DeltaTimeSeconds;
	auto RawNewYaw = RelativeRotation.Yaw + YawChange;

	SetRelativeRotation(FRotator(0, RawNewYaw, 0));
}