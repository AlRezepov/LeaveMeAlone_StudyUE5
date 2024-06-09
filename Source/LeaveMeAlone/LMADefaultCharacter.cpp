// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "LMADefaultCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <LMAPlayerController.h>
#include <BaseHUD.h>

// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(
		true); // данное условие не позволит нашей камере поворачиватьс€ в момент поворота персонажа.
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(
		FRotator(YRotation, 0.0f, 0.0f)); // структура FRotator хранит аргументы в следующей последовательности : Pitch, Yaw, Roll.“ак как
										  // нам необходимо определить значени€ по оси Y, мы устанавливаем Pitch аргумент.
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false; // данное условие запрещаем камере вращатьс€ относительно SpringArmComponent.

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	HealthComponent = CreateDefaultSubobject<UALMAHealthComponent>("HealthComponent");

	WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("Weapon");


}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}

	OnHealthChanged(HealthComponent->GetHealth());

	// ѕодписка на делегат FOnHealthChanged
	HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged);

	// ѕодписка на делегат FOnDeath
	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);


}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!(HealthComponent->IsDead()))
	{
		RotationPlayerOnCursor();
	}

	// ѕроверка уровн€ Stamina
	FString StaminaString = FString::Printf(TEXT("Stamina: %.2f"), Stamina);
	float TimeToDisplay = 2.0f;
	FColor StaminaColor = FColor::Red;
	int32 StaminaKey = 1;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(StaminaKey, TimeToDisplay, StaminaColor, StaminaString);
	}

	// ѕроверка, движетс€ ли персонаж вперЄд
	UpdateMovementStatus();
	
	//”слови€ траты и восполнени€ Stamina
	StaminaControl();

}

// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ALMADefaultCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &ALMADefaultCharacter::ZoomOut);

	
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaultCharacter::SprintOn);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaultCharacter::SprintOff);


	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &ULMAWeaponComponent::FireOff);

	
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::OnBulletsFinished);
}

void ALMADefaultCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}
void ALMADefaultCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::ZoomIn() 
{
	if (ArmLength <= MaxArmLength && ArmLength > MinArmLength)
	{
		SpringArmComponent->TargetArmLength -= 50.0f;
		ArmLength = SpringArmComponent->TargetArmLength;
	}
	
}

void ALMADefaultCharacter::ZoomOut() 
{
	if (ArmLength < MaxArmLength && ArmLength >= MinArmLength)
	{
		SpringArmComponent->TargetArmLength += 50.0f;
		ArmLength = SpringArmComponent->TargetArmLength;
	}
}

void ALMADefaultCharacter::SprintOn() 
{
	if (GetInputAxisValue("MoveForward") > 0.0f && Stamina > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
		Running = true;
	}
	else
	{
		Running = false;
	}
}

void ALMADefaultCharacter::SprintOff()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	Running = false;
}

void ALMADefaultCharacter::StaminaControl() 
{
	if (Running)
	{
		if (Stamina > 0)
		{
			Stamina -= 0.5;
		}
		else
		{
			SprintOff();
		}
	}
	else if (!Running)
	{
		if (Stamina < 100)
		{
			Stamina += 1;
		}
	}
}

void ALMADefaultCharacter::UpdateMovementStatus()
{
	// ѕроверка, движетс€ ли персонаж вперЄд
	if (GetInputAxisValue("MoveForward") == 0.0f)
	{
		SprintOff();
	}
}

void ALMADefaultCharacter::OnDeath() 
{
	CurrentCursor->DestroyRenderState_Concurrent();

	auto PlayerController = Cast<ALMAPlayerController>(Controller);
	if (PlayerController)
	{
		auto HUD = Cast<ABaseHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			for (auto Widget : HUD->WidgetContainer)
			{
				Widget->RemoveFromParent();
			}
		}
	}

	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(50.0f);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ALMADefaultCharacter::RotationPlayerOnCursor() 
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw; SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f))); 
		if (CurrentCursor) 
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location); 
		}
	}
}

bool ALMADefaultCharacter::IsRunning() const 
{
	return Running;
}
