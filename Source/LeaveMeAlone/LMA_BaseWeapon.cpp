// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "LMA_BaseWeapon.h"
#include <Kismet/GameplayStatics.h>
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
ALMA_BaseWeapon::ALMA_BaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponSkeletalMeshComponent);

	

}

// Called when the game starts or when spawned
void ALMA_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void ALMA_BaseWeapon::Fire(bool OnFire) 
{
	this->IsFire = OnFire;
	if (IsFire && !IsTimerActive)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ALMA_BaseWeapon::Shoot, ShootInterval, true);
		IsTimerActive = true;
	}
	else if (!IsFire && IsTimerActive)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		IsTimerActive = false;
	}

}

void ALMA_BaseWeapon::Shoot()
{
	const FTransform SocketTransform = WeaponSkeletalMeshComponent->GetSocketTransform("Muzzle");
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	FVector TracerEnd = TraceEnd;
	if (HitResult.bBlockingHit)
	{
		TracerEnd = HitResult.ImpactPoint;
	}
	SpawnTrace(TraceStart, TracerEnd);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootWave, TraceStart);
	DecrementBullets();
}

void ALMA_BaseWeapon::SpawnTrace(const FVector& TraceStart, const FVector& TraceEnd) 
{
	const auto TraceFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceEffect, TraceStart);
	if (TraceFX)
	{
		TraceFX->SetNiagaraVariableVec3(TraceName, TraceEnd);
	}
}

void ALMA_BaseWeapon::DecrementBullets() 
{
	
	CurrentAmmoWeapon.Bullets -= 1;

	if (IsCurrentClipEmpty() && !IsCurrentClipFull())
	{
		OnBulletsFinished.Broadcast();
		ChangeClip();
	}

}

bool ALMA_BaseWeapon::IsCurrentClipEmpty()
{
	return CurrentAmmoWeapon.Bullets == 0;
}

bool ALMA_BaseWeapon::IsCurrentClipFull()
{
	return CurrentAmmoWeapon.Bullets == 30;
}

void ALMA_BaseWeapon::ChangeClip() 
{
	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
}

// Called every frame
void ALMA_BaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Fire(IsFire);
}

