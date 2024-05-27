// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ALMAHealthComponent.h"
#include "LMAWeaponComponent.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UDecalComponent;
class UMaterialInterface;
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMADefaultCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	UALMAHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components") 
	ULMAWeaponComponent* WeaponComponent;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	// Функция для Sprint
	UFUNCTION(BlueprintCallable)
	void SprintOn();

	UFUNCTION(BlueprintCallable)
	void SprintOff();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	bool Running = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Функция при срабатывании делегата OnHealthChanged
	void OnHealthChanged(float NewHealth); 


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION() 
	UALMAHealthComponent* GetHealthComponent() const { return HealthComponent; } 


private:
	float YRotation = -75.0f; // отвечает за поворот камеры по оси Y.

	float ArmLength = 1400.0f; // отвечает за длину штатива.

	float FOV = 55.0f; // отвечает за поле зрения камеры.

	void MoveForward(float Value); // будет отвечать за движение персонажа по оси X.
	void MoveRight(float Value);   // будет отвечать за движение персонажа по оси Y.

	// Функции для изменения масштаба
	void ZoomIn();
	void ZoomOut();

	// Переменные для изменения масштаба
	float MaxArmLength = 1400.0f;
	float MinArmLength = 400.0f;

	// Переменные для скорости перемещения персонажа 
	float MaxSpeed = 800.0f;
	float NormalSpeed = 300.0f;

	//Параметр и функция выносливости
	void StaminaControl();
	void UpdateMovementStatus();
	float Stamina = 100.0f;

	//Смерть персонажа
	void OnDeath();

	//Отключение интерпретации персонажа относительно положения курсора.
	void RotationPlayerOnCursor();


};
