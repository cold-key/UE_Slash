#include "Characters/SlashAnimInstance.h"

#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	UpdateCharacterData();
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	UpdateCharacterData();
}

void USlashAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
	SlashCharacter = nullptr;
	ResetCharacterData();
}

void USlashAnimInstance::UpdateCharacterData()
{
	APawn* Pawn = TryGetPawnOwner();
	if (!IsValid(SlashCharacter) || SlashCharacter.Get() != Pawn)
	{
		SlashCharacter = Cast<ASlashCharacter>(Pawn);
	}

	if (!IsValid(SlashCharacter))
	{
		ResetCharacterData();
		return;
	}

	const UCharacterMovementComponent* Movement = SlashCharacter->GetCharacterMovement();
	Speed = IsValid(Movement) ? Movement->Velocity.Size2D() : 0.0;
	isFalling = IsValid(Movement) && Movement->IsFalling();
	EquipState = SlashCharacter->GetEquipState();
	AttackState = SlashCharacter->GetAttackState();
}

void USlashAnimInstance::ResetCharacterData()
{
	Speed = 0.0;
	isFalling = false;
	EquipState = ESlashCharacterEquipState::Unequiped;
	AttackState = ESlashCharacteAttackState::NoAttacking;
}
