// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter() :
PlayerMovementFactor(30),
bSelectPressed(false),
PlayerScore(0),
JumpCounter(0),
JumpHeight(100),
DashDistance(200)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetConstraintMode(EDOFMode::YZPlane);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, -1, FColor::Red, FString::Printf(TEXT("Score : %f"), PlayerScore));
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move", this, &APlayerCharacter::Movement);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::PlayerJump);
	
	PlayerInputComponent->BindAction("Select", IE_Pressed, this, &APlayerCharacter::SelectPressed);
	PlayerInputComponent->BindAction("Select", IE_Pressed, this, &APlayerCharacter::SelectReleased);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::Dash);
	
}

void APlayerCharacter::Movement(float Value)
{
	// FVector movementVector{0, Value, 0};
	//
	// AddMovementInput(movementVector* PlayerMovementFactor* GetWorld()->DeltaTimeSeconds);

	if(Controller!=nullptr && Value!=0)
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation{0, Rotation.Yaw, 0};

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, -Value*PlayerMovementFactor*GetWorld()->DeltaTimeSeconds);
	}
}

void APlayerCharacter::PlayerJump()
{
	if(JumpCounter<=1)
	{
		ACharacter::LaunchCharacter(FVector(0,0,JumpHeight), false, true);
		JumpCounter++;
	}
}

void APlayerCharacter::SelectPressed()
{
	bSelectPressed = true;
}

void APlayerCharacter::SelectReleased()
{
	bSelectPressed = false;
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	JumpCounter = 0;
}

void APlayerCharacter::Dash()
{
	GetCharacterMovement()->BrakingFriction = 0;
	ACharacter::LaunchCharacter(FVector(0, DashDistance, 0), true, true);
}


