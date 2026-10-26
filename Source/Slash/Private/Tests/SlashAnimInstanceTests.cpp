#if WITH_DEV_AUTOMATION_TESTS

#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "UObject/UnrealType.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSlashAnimationCharacterDataTest,
	"Slash.Animation.CharacterData",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FSlashAnimationCharacterDataTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	if (!TestNotNull(TEXT("Test world"), World))
	{
		return false;
	}
	ON_SCOPE_EXIT
	{
		World->DestroyWorld(false);
	};

	ASlashCharacter* Character = World->SpawnActor<ASlashCharacter>();
	if (!TestNotNull(TEXT("Native character without a character Blueprint"), Character))
	{
		return false;
	}

	USlashAnimInstance* Instance = NewObject<USlashAnimInstance>(Character->GetMesh());
	const UClass* Class = USlashAnimInstance::StaticClass();
	const FDoubleProperty* SpeedProperty = FindFProperty<FDoubleProperty>(Class, TEXT("Speed"));
	const FBoolProperty* FallingProperty = FindFProperty<FBoolProperty>(Class, TEXT("isFalling"));
	const FEnumProperty* EquipProperty = FindFProperty<FEnumProperty>(Class, TEXT("EquipState"));
	const FEnumProperty* AttackProperty = FindFProperty<FEnumProperty>(Class, TEXT("AttackState"));
	const FObjectPropertyBase* CharacterProperty = FindFProperty<FObjectPropertyBase>(Class, TEXT("SlashCharacter"));
	if (!TestNotNull(TEXT("Speed remains a Blueprint double"), SpeedProperty)
		|| !TestNotNull(TEXT("Falling property"), FallingProperty)
		|| !TestNotNull(TEXT("Equip property"), EquipProperty)
		|| !TestNotNull(TEXT("Attack property"), AttackProperty)
		|| !TestNotNull(TEXT("Native character reference"), CharacterProperty))
	{
		return false;
	}

	auto ReadEnum = [Instance](const FEnumProperty* Property)
	{
		return Property->GetUnderlyingProperty()->GetUnsignedIntPropertyValue(
			Property->ContainerPtrToValuePtr<void>(Instance));
	};

	Instance->NativeInitializeAnimation();
	TestEqual(TEXT("Caches the native owner"),
		CharacterProperty->GetObjectPropertyValue_InContainer(Instance), static_cast<UObject*>(Character));
	TestEqual(TEXT("Starts unequipped"), ReadEnum(EquipProperty), uint64(ESlashCharacterEquipState::Unequiped));

	UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
	Movement->Velocity = FVector(300.f, 400.f, 900.f);
	Movement->SetMovementMode(MOVE_Falling);
	Character->SetEquipState(ESlashCharacterEquipState::Equiped);
	Character->SetAttackState(ESlashCharacteAttackState::Attacking);
	Instance->NativeUpdateAnimation(1.f / 60.f);
	TestEqual(TEXT("Locomotion speed excludes vertical velocity"), SpeedProperty->GetPropertyValue_InContainer(Instance), 500.0);
	TestTrue(TEXT("Reads falling movement mode"), FallingProperty->GetPropertyValue_InContainer(Instance));
	TestEqual(TEXT("Reads equipped state"), ReadEnum(EquipProperty), uint64(ESlashCharacterEquipState::Equiped));
	TestEqual(TEXT("Reads attacking state"), ReadEnum(AttackProperty), uint64(ESlashCharacteAttackState::Attacking));

	Movement->Velocity = FVector::ZeroVector;
	Movement->SetMovementMode(MOVE_Walking);
	Character->SetAttackState(ESlashCharacteAttackState::NoAttacking);
	Instance->NativeUpdateAnimation(1.f / 30.f);
	TestEqual(TEXT("Stopping clears speed"), SpeedProperty->GetPropertyValue_InContainer(Instance), 0.0);
	TestFalse(TEXT("Landing clears falling"), FallingProperty->GetPropertyValue_InContainer(Instance));
	TestEqual(TEXT("Attack end is reflected next update"), ReadEnum(AttackProperty), uint64(ESlashCharacteAttackState::NoAttacking));

	Instance->NativeUninitializeAnimation();
	TestNull(TEXT("Uninitialization clears owner"), CharacterProperty->GetObjectPropertyValue_InContainer(Instance));
	TestEqual(TEXT("Uninitialization resets equipment"), ReadEnum(EquipProperty), uint64(ESlashCharacterEquipState::Unequiped));

	USkeletalMeshComponent* PreviewMesh = NewObject<USkeletalMeshComponent>();
	USlashAnimInstance* Preview = NewObject<USlashAnimInstance>(PreviewMesh);
	Preview->NativeInitializeAnimation();
	Preview->NativeUpdateAnimation(1.f / 60.f);
	TestEqual(TEXT("Ownerless preview has zero speed"), SpeedProperty->GetPropertyValue_InContainer(Preview), 0.0);
	TestFalse(TEXT("Ownerless preview is not falling"), FallingProperty->GetPropertyValue_InContainer(Preview));
	TestNull(TEXT("Ownerless preview has no character"), CharacterProperty->GetObjectPropertyValue_InContainer(Preview));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSlashAnimationBlueprintIntegrationTest,
	"Slash.Animation.BlueprintIntegration",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FSlashAnimationBlueprintIntegrationTest::RunTest(const FString& Parameters)
{
	UClass* CharacterClass = LoadClass<ASlashCharacter>(nullptr,
		TEXT("/Game/Blueprints/Characters/BP_SlashCharacter.BP_SlashCharacter_C"));
	if (!TestNotNull(TEXT("Existing character Blueprint loads"), CharacterClass))
	{
		return false;
	}

	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	ON_SCOPE_EXIT
	{
		World->DestroyWorld(false);
	};
	ASlashCharacter* Character = World->SpawnActor<ASlashCharacter>(CharacterClass);
	if (!TestNotNull(TEXT("Existing character Blueprint spawns"), Character))
	{
		return false;
	}

	USkeletalMeshComponent* Mesh = Character->GetMesh();
	USlashAnimInstance* Instance = Cast<USlashAnimInstance>(Mesh->GetAnimInstance());
	if (!TestNotNull(TEXT("Existing animation Blueprint uses the native parent"), Instance))
	{
		return false;
	}

	Character->GetCharacterMovement()->Velocity = FVector(300.f, 400.f, 900.f);
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	Character->SetEquipState(ESlashCharacterEquipState::Equiped);
	Character->SetAttackState(ESlashCharacteAttackState::Attacking);
	Mesh->TickAnimation(1.f / 60.f, false);
	Mesh->RefreshBoneTransforms();
	const FDoubleProperty* SpeedProperty = FindFProperty<FDoubleProperty>(Instance->GetClass(), TEXT("Speed"));
	TestEqual(TEXT("Animation graph receives native movement data"), SpeedProperty->GetPropertyValue_InContainer(Instance), 500.0);

	UFunction* AttackEnd = Instance->FindFunction(TEXT("AnimNotify_AttackEnd"));
	if (TestNotNull(TEXT("Existing attack end notification remains"), AttackEnd))
	{
		Instance->ProcessEvent(AttackEnd, nullptr);
		TestEqual(TEXT("Attack end notification still reaches the character"),
			Character->GetAttackState(), ESlashCharacteAttackState::NoAttacking);
	}

	Character->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Mesh->TickAnimation(1.f / 60.f, false);
	Mesh->RefreshBoneTransforms();
	TestEqual(TEXT("Graph also evaluates after landing and stopping"), SpeedProperty->GetPropertyValue_InContainer(Instance), 0.0);
	return true;
}

#endif
