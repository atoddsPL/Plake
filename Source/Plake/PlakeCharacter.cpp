// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlakeCharacter.h"

//GAME
#include "PlakePlayerController.h"

//ENGINE
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

//////////////////////////////////////////////////////////////////////////
// APlakeCharacter

APlakeCharacter::APlakeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bReplicates = true;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0, 80.f, 100.f);
	

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	SpellBookComponent = CreateDefaultSubobject<USpellBookComponent>(TEXT("SpellBook"));

	ProjectileSpawnArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Spell spawn arrow"));
	ProjectileSpawnArrowComponent->SetupAttachment(RootComponent);


	PlayerStatsComponent = CreateDefaultSubobject<UPlayerStatsComponent>(TEXT("Stats"));
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void APlakeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Cast", IE_Pressed, this, &APlakeCharacter::CastStart);
	PlayerInputComponent->BindAction("Cast", IE_Released, this, &APlakeCharacter::CastEnd);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlakeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlakeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlakeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlakeCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APlakeCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APlakeCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &APlakeCharacter::OnResetVR);
}

void APlakeCharacter::CastStart()
{
	if (!SpellBookComponent) return;
	float ManaCost = SpellBookComponent->GetActiveManaCost();

	// dorobic check na mane
	if (true)
	{
		SpellBookComponent->StartCast();
	}
}

void APlakeCharacter::CastEnd()
{
}

void APlakeCharacter::TryUpdateUI_Implementation()
{
	if (!GEngine) return;

	APlakePlayerController* PC = Cast<APlakePlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (!PC) return;
	PC->UpdateUI(PlayerStatsComponent);
}

float APlakeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Damage taken %f"), DamageAmount);
	if (!PlayerStatsComponent) { return 0.0f; }

	if (!PlayerStatsComponent->ChangeHealth(-DamageAmount))
	{
		// Obs³uga deatha!
	}
	
	TryUpdateUI();
	return 0.0f;
}

bool APlakeCharacter::ManaCheck(float ManaCost)
{

	if (!PlayerStatsComponent) return false;
	if (PlayerStatsComponent->GetCurrentMana() - ManaCost >= 0) return true;
	return false;
}


void APlakeCharacter::OnResetVR()
{
	// If Plake is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in Plake.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APlakeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void APlakeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void APlakeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlakeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlakeCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlakeCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
