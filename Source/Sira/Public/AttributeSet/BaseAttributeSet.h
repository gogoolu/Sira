// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS(BlueprintType, Blueprintable)
class SIRA_API UEnemyAttr : public UAttributeSet
{
	GENERATED_BODY()
public:
	// 血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseAttributeSet")	
	FGameplayAttributeData HP;
	ATTRIBUTE_ACCESSORS(UEnemyAttr, HP);
	// 血量上限
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseAttributeSet")
	FGameplayAttributeData MaxHP;
	ATTRIBUTE_ACCESSORS(UEnemyAttr, MaxHP);
	// 基础伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseAttributeSet")
	FGameplayAttributeData BaseDamage;
	ATTRIBUTE_ACCESSORS(UEnemyAttr, BaseDamage);
	// %伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseAttributeSet")
	FGameplayAttributeData DamagePercent;
	ATTRIBUTE_ACCESSORS(UEnemyAttr, DamagePercent);
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};


UCLASS(BlueprintType, Blueprintable)
class SIRA_API UPlayerAttr : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	// 血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPlayerBaseAttr")
	FGameplayAttributeData HP = 100.f;
	ATTRIBUTE_ACCESSORS(UPlayerAttr, HP);
	// 血量上限
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPlayerBaseAttr")
	FGameplayAttributeData MaxHP = 100.f;
	ATTRIBUTE_ACCESSORS(UPlayerAttr, MaxHP);
	// 基础伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPlayerBaseAttr")
	FGameplayAttributeData BaseDamage = 10.f;
	ATTRIBUTE_ACCESSORS(UPlayerAttr, BaseDamage);
	// %攻击速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPlayerBaseAttr")
	FGameplayAttributeData AttackSpeedPercent = 0.f;
	ATTRIBUTE_ACCESSORS(UPlayerAttr, AttackSpeedPercent);
	// 护甲:		ReduceDamagePercent = 1 / ( 1 + ( ARMOR / 15 ) ), if Armor <= 0, no effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPlayerBaseAttr")
	FGameplayAttributeData Armor = 0.f;
	ATTRIBUTE_ACCESSORS(UPlayerAttr, Armor);
	// // 抢夺:		
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPlayerBaseAttr")
	// FGameplayAttributeData Looting = 0.f;
	// ATTRIBUTE_ACCESSORS(UPlayerAttr, Looting);
	// 生命回复:	HPEveryXSeconds = 5.0 / ( 1.0 + ( ( HP_Regen - 1) / 2.25 ) )
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPlayerBaseAttr")
	FGameplayAttributeData HPRegen = 0.f;
	ATTRIBUTE_ACCESSORS(UPlayerAttr, HPRegen);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};


UCLASS(BlueprintType, Blueprintable)
class SIRA_API UPlayerModAttr : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	// damage = (BaseDamage + FlatDamageBonus) * (1 + DamagePercent)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPlayerModAttr", meta = (ToolTips = "+伤害"))
	FGameplayAttributeData FlatDamageBonus = 0.f;
	ATTRIBUTE_ACCESSORS(UPlayerModAttr, FlatDamageBonus);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPlayerModAttr", meta = (ToolTips = "%伤害"))
	FGameplayAttributeData DamagePercent = 0.f;
	ATTRIBUTE_ACCESSORS(UPlayerModAttr, DamagePercent);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPlayerModAttr", meta = (ToolTips = "%暴击率"))
	FGameplayAttributeData CriticalRate = 0.25f;
	ATTRIBUTE_ACCESSORS(UPlayerModAttr, CriticalRate);
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};