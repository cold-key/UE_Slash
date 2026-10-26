#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/SlashCharacterEnum.h"
#include "SlashAnimInstance.generated.h"

class ASlashCharacter;

/** Prepares character data for the animation graph on the game thread. */
UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeUninitializeAnimation() override;

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation|Character")
	TObjectPtr<ASlashCharacter> SlashCharacter = nullptr;

	// Keep the existing Blueprint property names and types for the animation graph.
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation|Movement")
	double Speed = 0.0;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation|Movement")
	bool isFalling = false;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation|Character")
	ESlashCharacterEquipState EquipState = ESlashCharacterEquipState::Unequiped;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation|Character")
	ESlashCharacteAttackState AttackState = ESlashCharacteAttackState::NoAttacking;

private:
	void UpdateCharacterData();
	void ResetCharacterData();
};
