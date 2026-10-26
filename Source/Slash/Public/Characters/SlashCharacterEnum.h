#pragma once

#include "CoreMinimal.h"
#include "SlashCharacterEnum.generated.h"

UENUM(BlueprintType)
enum class ESlashCharacterEquipState : uint8
{
    Equiped   UMETA(DisplayName = "Equipped"),
    Unequiped UMETA(DisplayName = "Unequipped")
};

UENUM(BlueprintType)
enum class ESlashCharacteAttackState : uint8
{
    NoAttacking   UMETA(DisplayName = "NoAttacking"),
    Attacking UMETA(DisplayName = "Attacking")
};
