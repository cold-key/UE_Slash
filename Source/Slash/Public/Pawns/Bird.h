// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"

class USkeletalMeshComponent;
class UFloatingPawnMovement;
class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;

UCLASS()
class SLASH_API ABird : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABird();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void TurnByMouse(float value);
	void UpByMouse(float value);

private:	

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* skCom;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* capsuleCom;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* moveCom;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* springArmCom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* cameraCom;

};
