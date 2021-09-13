// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "TankogeddonHW2.h"

#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values
ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);


}

void ACannon::Fire()
{
	if (!IsReadyToFire())
	{
		return;
	}
	bReadyToFire = false;
	--NumAmmo;

	ShotsLeft = NumShotsInSeries;
	Shot();
}

void ACannon::FireSpecial()
{
	if (!HasSpecialFire() || !IsReadyToFire())
	{
		return;
	}

	bReadyToFire = false;
	--NumAmmo;

	if (Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, TEXT("Fire special - projectile"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, TEXT("Fire special - trace"));
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
	//UE_LOG(LogTankogeddon, Log, TEXT("FireSpecial! Ammo left: %d"), NumAmmo);
}

bool ACannon::IsReadyToFire() const
{
	return bReadyToFire && NumAmmo > 0 && ShotsLeft == 0;
}

bool ACannon::HasSpecialFire() const
{
	return bHasSpecialFire;
}

void ACannon::Reload()
{
	bReadyToFire = true;
}


// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	bReadyToFire = true;
	ShotsLeft = 0;
	NumAmmo = MaxAmmo;
	
}

void ACannon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
Super::EndPlay(EndPlayReason);

GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
GetWorld()->GetTimerManager().ClearTimer(SeriesTimerHandle);
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACannon::Shot()
{
	check(ShotsLeft > 0)
		if (Type == ECannonType::FireProjectile)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1, FColor::Green, TEXT("Fire - projectile"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1, FColor::Green, TEXT("Fire - trace"));
		}

	if (--ShotsLeft > 0)
	{
		const float NextShotTime = SeriesLength / (NumShotsInSeries - 1);
		GetWorld()->GetTimerManager().SetTimer(SeriesTimerHandle, this, &ACannon::Shot, NextShotTime, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
	}
}

