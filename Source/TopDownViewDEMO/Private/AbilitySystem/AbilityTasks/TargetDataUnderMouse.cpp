	// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

	UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	//IsLocallyControlled 判断这次技能对应的角色 是不是由当前这台机器上的本地玩家控制
	//获取鼠标位置这件事 应该由真正操作鼠标的本地玩家来做
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		//如果这个角色由本地玩家控制 就读取他的鼠标位置 并发送目标数据
		SendMouseCursorData();
	}
	else
	{
		//如果当前不是本地玩家控制的角色 就应该准备接收客户端传来的目标数据
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();//哪个技能
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();//哪一次技能激活
		//当这次技能激活对应的目标数据到达时，请调用我的 OnTargetDataReplicatedCallback() 函数
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	/*
	 * 打开预测窗口
	 * 在 GAS 中 客户端有时会预测某些技能操作 预测键（Prediction Key）用于关联这些操作 
	 * 这一步是为了接下来的目标数据发送操作 准备好 GAS 需要的预测环境
	 */
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	
	//获取鼠标命中结果
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	/** 将目标数据复制到服务器 */
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(), //指明是哪一个已授予的技能
		GetActivationPredictionKey(), //标识这次技能激活 用于匹配对应的目标数据
		DataHandle, //真正要发送的目标数据
		FGameplayTag(), //这里没有附加额外的应用标签
		AbilitySystemComponent->ScopedPredictionKey);//提供当前预测窗口关联的预测键
		
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
	
}
