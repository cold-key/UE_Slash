// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SlashCharacterEnum.h"
#include "SlashCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetItemPtr(AItem* InItem) { itemPtr = InItem; };

	UFUNCTION(BlueprintPure)
	ESlashCharacterEquipState GetEquipState() { return equipState; };

	UFUNCTION(BlueprintCallable)
	void SetEquipState(ESlashCharacterEquipState nextState) { equipState = nextState; };

	UFUNCTION(BlueprintPure)
	ESlashCharacteAttackState GetAttackState() { return attackState; };

	UFUNCTION(BlueprintCallable)
	void SetAttackState(ESlashCharacteAttackState nextState) { attackState = nextState; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveToForward(float value);
	void MoveToRight(float value);
	void TurnViewByMouse(float value);
	void UpViewByMouse(float value);
	void JumpBySpaceBar();
	void onPressE();
	void onAttack();

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

	UPROPERTY(VisibleAnywhere)
	ESlashCharacterEquipState equipState = ESlashCharacterEquipState::Unequiped;

	UPROPERTY(VisibleAnywhere)
	ESlashCharacteAttackState attackState = ESlashCharacteAttackState::NoAttacking;

	UPROPERTY(EditDefaultsOnly,Category = "Montage")
	UAnimMontage* attackMontage;

};
