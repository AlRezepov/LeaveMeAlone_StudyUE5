// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMA_BaseWeapon.h"
#include "GameFramework/Character.h"
#include "LMAReloadFinishedAnimNotify.h"
#include "LMAWeaponComponent.generated.h"

class ALMADefaultCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEAVEMEALONE_API ULMAWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULMAWeaponComponent();

	void Fire();
	void Reload();
	bool CanReload() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class ALMA_BaseWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName SocketName;

	UPROPERTY()
	ALMA_BaseWeapon* Weapon = nullptr;

	void SpawnWeapon();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadMontage;

	bool AnimReloading = false;

	void InitAnimNotify();
	void OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
