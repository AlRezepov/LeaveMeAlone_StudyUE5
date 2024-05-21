// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(
		true); // данное условие не позволит нашей камере поворачиваться в момент поворота персонажа.
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(
		FRotator(YRotation, 0.0f, 0.0f)); // структура FRotator хранит аргументы в следующей последовательности : Pitch, Yaw, Roll.Так как
										  // нам необходимо определить значения по оси Y, мы устанавливаем Pitch аргумент.
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false; // данное условие запрещаем камере вращаться относительно SpringArmComponent.

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
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
	if (GetInputAxisValue("MoveForward") > 0.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
		Running = true;
	}
}

void ALMADefaultCharacter::SprintOff()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	Running = false;
}
