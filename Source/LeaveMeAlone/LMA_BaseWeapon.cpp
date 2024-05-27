// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "LMA_BaseWeapon.h"

// Sets default values
ALMA_BaseWeapon::ALMA_BaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);

}

// Called when the game starts or when spawned
void ALMA_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALMA_BaseWeapon::Shoot() 
{
	const FTransform SocketTransform = WeaponComponent->GetSocketTransform("Muzzle");
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
}

void ALMA_BaseWeapon::Fire()
{
	Shoot();
}

// Called every frame
void ALMA_BaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

