// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "LMADamageActor.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ALMADamageActor::ALMADamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(SphereRadius);
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	
	
	RootComponent = SphereComponent;
	ParticleSystemComponent->SetupAttachment(SphereComponent);



}

// Called when the game starts or when spawned
void ALMADamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALMADamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), SphereRadius, nullptr, {}, this, nullptr, false);
}

