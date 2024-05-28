// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "LMA_BaseWeapon.h"
#include "LMAWeaponComponent.h"


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
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
	}

	DecrementBullets();
}

void ALMA_BaseWeapon::DecrementBullets() 
{
	
	CurrentAmmoWeapon.Bullets -= 1;

	if (IsCurrentClipEmpty())
	{
		ChangeClip();
	}

	//Отображение кол-ва патронов
	FString BulletsString = FString::Printf(TEXT("Bullets: %d"), CurrentAmmoWeapon.Bullets);
	float TimeToDisplay = 2.0f;
	FColor BulletsColor = FColor::Blue;
	int32 BulletsKey = 2;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(BulletsKey, TimeToDisplay, BulletsColor, BulletsString);
	}
}

bool ALMA_BaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets == 0;
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

