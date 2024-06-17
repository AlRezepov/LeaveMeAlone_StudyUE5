// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMA_BaseWeapon.generated.h"

class USkeletalMeshComponent;
class USoundWave;
class UNiagaraSystem;

DECLARE_MULTICAST_DELEGATE(FOnBulletsFinished);

USTRUCT(BlueprintType) struct FAmmoWeapon
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite;
};

UCLASS()
class LEAVEMEALONE_API ALMA_BaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALMA_BaseWeapon();

	void Fire(bool OnFire);
	void ChangeClip();

	FOnBulletsFinished OnBulletsFinished;

	bool IsCurrentClipFull();

	FAmmoWeapon GetCurrentAmmoWeapon() const { return CurrentAmmoWeapon; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Shoot();
	bool IsFire = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UNiagaraSystem* TraceEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FString TraceName = "Tracer";

	void SpawnTrace(const FVector& TraceStart, const FVector& TraceEnd);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	USoundWave* ShootWave;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ShootInterval = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponSkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoWeapon AmmoWeapon{30, 0, true};

	FAmmoWeapon CurrentAmmoWeapon = AmmoWeapon;

	void DecrementBullets();
	bool IsCurrentClipEmpty();

private:
	
	//Переменные таймера
	FTimerHandle TimerHandle;
	bool IsTimerActive;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
