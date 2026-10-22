// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include<Camera/CameraComponent.h>
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"


// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	springArmCom = CreateDefaultSubobject<USpringArmComponent>(FName("springArmCom"));
	springArmCom->SetupAttachment(RootComponent);

	cameraCom = CreateDefaultSubobject<UCameraComponent>(FName("cameraCom"));
	cameraCom->SetupAttachment(springArmCom);

	hair = CreateDefaultSubobject<UGroomComponent>(FName("hair"));
	hair->SetupAttachment(GetMesh());
	hair->AttachmentName = FString("head");

	eyebrows = CreateDefaultSubobject<UGroomComponent>(FName("eyebrows"));
	eyebrows->SetupAttachment(GetMesh());
	eyebrows->AttachmentName = FString("head");
}

// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveToForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASlashCharacter::MoveToRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::TurnViewByMouse);
	PlayerInputComponent->BindAxis(FName("Up"), this, &ASlashCharacter::UpViewByMouse);

	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &ASlashCharacter::JumpBySpaceBar);
	PlayerInputComponent->BindAction(FName("PressE"), EInputEvent::IE_Pressed, this, &ASlashCharacter::onPressE);

}

void ASlashCharacter::MoveToForward(float value)
{
	if (Controller && value != 0.f) {
		FRotator controlRotation = GetControlRotation();
		FRotator yawRotation = FRotator(0.f, controlRotation.Yaw, 0.f);
		FVector v =  FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(v, value);
	}
}

void ASlashCharacter::MoveToRight(float value)
{
	if (Controller && value != 0.f) {
		FRotator controlRotation = GetControlRotation();
		FRotator yawRotation = FRotator(0.f, controlRotation.Yaw, 0.f);
		FVector v = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(v, value);
	}
}

void ASlashCharacter::TurnViewByMouse(float value)
{
	if (Controller && value != 0.f) {
		AddControllerYawInput(value);
	}
}

void ASlashCharacter::UpViewByMouse(float value)
{
	if (Controller && value != 0.f) {
		AddControllerPitchInput(value);
	}
}

void ASlashCharacter::JumpBySpaceBar()
{
	//Super::Jump();
	Jump();
}

void ASlashCharacter::onPressE()
{
	if (itemPtr) {
		FAttachmentTransformRules rules (EAttachmentRule::SnapToTarget,false);
		itemPtr->AttachToComponent(GetMesh(),rules,FName("RightHand"));
	}
}

