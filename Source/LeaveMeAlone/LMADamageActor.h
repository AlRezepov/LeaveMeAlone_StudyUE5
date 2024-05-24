// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/DrawSphereComponent.h>
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "LMADamageActor.generated.h"

class UParticleSystemComponent;

UCLASS()
class LEAVEMEALONE_API ALMADamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMADamageActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage") 
	float Damage = 5.0f;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage") 
	float SphereRadius = 100.0f; 



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
