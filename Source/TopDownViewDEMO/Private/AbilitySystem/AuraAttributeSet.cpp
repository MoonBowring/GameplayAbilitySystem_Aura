// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Chaos/Deformable/MuscleActivationConstraints.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMana(50.f);
	InitMaxHealth(100.f);
	InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	//表明Health属性需要网络登记   表明  哪个类的属性      哪个属性  这个属性有什么复制条件吗  复制的时候总是使用这个属性对应的通知机制
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());	
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
		const UAbilitySystemComponent* SourceASC = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
		
		if (IsValid(SourceASC) && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
		{
			AActor* SourceAvatarActor = SourceASC->AbilityActorInfo->AvatarActor.Get();
			const AController* SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();
			if (SourceController == nullptr && SourceAvatarActor != nullptr)
			{
				if (const APawn* Pawn = Cast<APawn>(SourceAvatarActor))
				{
					SourceController = Pawn->GetController();
				}
			}
			if (SourceController)
			{
				ACharacter* SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
			}
		}
	}
	
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		AController* TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetAvatarActor);
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetAvatarActor);
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	//GAMEPLAYATTRIBUTE_REPNOTIFY 专门用于处理AttributeSet属性的网络复制通知
	//告诉GAS这个Attribute刚刚发生了网络复制 需要按照GAS的属性变化机制处理这次变化
	//也就是在告诉能力系统 我们正在同步一个值 它的值刚从服务器下来并且改变了 现在能力系统能记录这个变化 并记录这个旧值 以备需要滚回时使用
	//这就是GAS作者提供的一个宏，帮助我们在“网络复制已经发生之后”，把这次属性变化正确地通知给GAS
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}
