// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	//获取到具体玩家的属性设置
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	//这是通过广播初始化玩家数值
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());//当 Health 发生改变时 从AttributeSet读取到当前Health 然后广播出去
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	//监听属性变化 申请一个专门监听某个 Attribute 变化的 Delegate
	//简单的来说 就是告诉 AbilitySystemComponent 以后 Health 这个属性发生变化时 就来调用 HealthChanged 这个函数
	//  GetGameplayAttributeValueChangeDelegate这个就像订阅公众号        监听那个属性 也就是订阅那个公众号              当 Health 发生变化时 也就是公众号更新时 请记下我的HealthChanged函数 也就是通知我 需要时调用
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)//把 TagContainer 里的每一个 GameplayTag 一个一个拿出来
			{
				//获取与指定标签名对应的游戏标签对象
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				//MatchesTag 判断此标签是否与要检查的标签相匹配 同时展开其父类标签 也就是判断当前这个 Tag 是不是属于 Message 这一大类
				//因为一个 GameplayEffect 可能带很多种 Tag 但是我只想处理 Message.xxx
				if (Tag.MatchesTag(MessageTag))
				{
					//拿着这个 Tag 去 UIDataTable 找对应的一行
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowSignature.Broadcast(*Row);
				}
			}
		}
	);	
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	//把 Data.NewValue 这个新血量 广播给所以已经监听 OnHealthChanged 的人 比如说 WBP_HealthGlobe
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
