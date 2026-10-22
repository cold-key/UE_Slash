// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "GameFramework/FloatingPawnMovement.h"
#include<Components/SkeletalMeshComponent.h>
#include<Camera/CameraComponent.h>
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleCom = CreateDefaultSubobject<UCapsuleComponent>(FName("capsuleCom"));
	SetRootComponent(capsuleCom);

	skCom = CreateDefaultSubobject<USkeletalMeshComponent>(FName("skCom"));
	skCom->SetupAttachment(RootComponent);

	moveCom = CreateDefaultSubobject<UFloatingPawnMovement>(FName("moveCom"));
	moveCom->SetUpdatedComponent(RootComponent);

	springArmCom = CreateDefaultSubobject<USpringArmComponent>(FName("springArmCom"));
	springArmCom->SetupAttachment(RootComponent);

	cameraCom = CreateDefaultSubobject<UCameraComponent>(FName("cameraCom"));
	cameraCom->SetupAttachment(springArmCom);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABird::MoveForward);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABird::TurnByMouse);
	PlayerInputComponent->BindAxis(FName("Up"), this, &ABird::UpByMouse);

}

void ABird::MoveForward(float value)
{
	if (Controller && value != 0.f) {
		FVector forwardVec = GetActorForwardVector();
		AddMovementInput(forwardVec, value);
	}
}

void ABird::TurnByMouse(float value)
{
	FString msg = FString::Printf(TEXT("Enter ABird::TurnByMouse, value is %f"),value);
	GEngine->AddOnScreenDebugMessage(3, 30.f, FColor::Cyan, msg);
	if (Controller && value != 0.f) {
		AddControllerYawInput(value);
	}
}

void ABird::UpByMouse(float value)
{
	if (Controller && value != 0.f) {
		AddControllerPitchInput(value);
	}
}
