// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "ALMAHealthComponent.h"

// Sets default values for this component's properties
UALMAHealthComponent::UALMAHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UALMAHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	AActor* OwnerComponent = GetOwner();
	if (OwnerComponent)
	{
		OwnerComponent->OnTakeAnyDamage.AddDynamic(this, &UALMAHealthComponent::OnTakeAnyDamage);
	}
}

void UALMAHealthComponent::OnTakeAnyDamage(
	AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	Health -= Damage;
}
