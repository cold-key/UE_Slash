// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Items/Item.h"
#include "SlashCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlashCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetItemPtr(AItem* InItem) { itemPtr = InItem; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveToForward(float value);
	void MoveToRight(float value);
	void TurnViewByMouse(float value);
	void UpViewByMouse(float value);
	void JumpBySpaceBar();
	void onPressE();

private:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* springArmCom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* cameraCom;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* hair;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* eyebrows;

	UPROPERTY(VisibleAnywhere)
	AItem* itemPtr;

};
