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

// Called every frame
void ALMA_BaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

