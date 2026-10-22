// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon.h"
#include<Components/SkeletalMeshComponent.h>

AWeapon::AWeapon()
{

}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//skCom->AddWorldOffset(FVector(0.f, 0.f, TransformSin()));
}
