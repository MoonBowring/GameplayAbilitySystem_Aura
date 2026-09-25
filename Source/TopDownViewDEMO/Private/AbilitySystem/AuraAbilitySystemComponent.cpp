// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//以后这个 ASC 收到 Gameplay Effect 的时候，调用我自己的 EffectApplied()
	//也就是告诉这个 Delegate 以后发生 OnGameplayEffectAppliedDelegateToSelf 的时候, 请通知我 并且调用我的 EffectApplied 函数
	//GameplayEffect 被应用到这个 ASC 时，这个 Delegate 被触发
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
