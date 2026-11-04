// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNVIEWDEMO_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
protected:
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	//FGameplayTag 伤害类型 FScalableFloat 用来保存伤害根据等级变化的数值
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
