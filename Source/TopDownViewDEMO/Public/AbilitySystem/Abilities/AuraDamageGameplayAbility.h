// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

UCLASS()
class TOPDOWNVIEWDEMO_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
protected:
	//造成什么效果
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	//FGameplayTag 伤害类型 FScalableFloat 用来保存伤害根据等级变化的数值
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
	
	//根据输入的标签蒙太奇数组来随机选择蒙太奇
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
