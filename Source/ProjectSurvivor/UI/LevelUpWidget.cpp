// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LevelUpWidget.h"

void ULevelUpWidget::SelectAttackDamage()
{
	OnAttackDamageSelected.Broadcast();
}

void ULevelUpWidget::SelectMoveSpeed()
{
	OnMoveSpeedSelected.Broadcast();
}

void ULevelUpWidget::SelectLaserWeapon()
{
	OnLaserWeaponSelected.Broadcast();
}

void ULevelUpWidget::SelectFireRate()
{
	OnFireRateSelected.Broadcast();
}

void ULevelUpWidget::SelectProjectileCount()
{
	OnProjectileCountSelected.Broadcast();
}