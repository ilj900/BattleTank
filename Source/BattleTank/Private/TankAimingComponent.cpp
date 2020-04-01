// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (Ammo <= 0)
	{
		FiringState = EFiringState::Empty;
		return;
	}
	if ((GetWorld()->GetTimeSeconds() - LastFireTime) < ReloadTime)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}

EFiringState UTankAimingComponent::GetFiringState()
{
	return FiringState;
}

int32 UTankAimingComponent::GetRoundsLeft()
{
	return Ammo;
}

void UTankAimingComponent::BeginPlay()
{
	LastFireTime = GetWorld()->GetTimeSeconds();
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!(Barrel))
		return;

	auto BarrelLocation = Barrel->GetSocketLocation("LaunchSocket");
	FVector TossVelocity;
	if (!UGameplayStatics::SuggestProjectileVelocity(
		this,
		TossVelocity,
		BarrelLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace))
		return;
	AimDirection = TossVelocity.GetSafeNormal();
	MoveBarrelTowards(AimDirection);
}

void UTankAimingComponent::MoveBarrelTowards(FVector DirectionToAim)
{
	if (!(Barrel))
		return;
	if (!(Turret))
		return;
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = DirectionToAim.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotation;
	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
		Turret->Rotate(DeltaRotator.Yaw);
	else
		Turret->Rotate(-DeltaRotator.Yaw);
}
void UTankAimingComponent::Fire()
{
	if (Ammo == 0)
		return;
	if (FiringState == EFiringState::Reloading)
		return;
	if (!(Barrel))
		return;
	if (!(ProjectileBlueprint))
		return;
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation("LaunchSocket"), Barrel->GetSocketRotation("LaunchSocket"));
	Projectile->LaunchProjectile(LaunchSpeed);
	LastFireTime = GetWorld()->GetTimeSeconds();
	--Ammo;
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!(Barrel))
		return false;
	auto BarrelDirection = Barrel->GetForwardVector();
	return !AimDirection.Equals(BarrelDirection, 0.01f);
}


