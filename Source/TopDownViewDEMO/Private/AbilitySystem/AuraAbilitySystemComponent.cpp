// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

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
	for (const FGameplayTag& Tag : TagContainer)//把 TagContainer 里的每一个 GameplayTag 一个一个拿出来
	{
		const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
	}
}
