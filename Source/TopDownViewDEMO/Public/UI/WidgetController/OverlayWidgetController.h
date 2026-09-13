// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAuraUserWidget;
struct FGameplayTag;
struct FOnAttributeChangeData;
//通过广播事件 实现对角色生命值 HP 的变化监听 利用该广播 通知所有绑定到该事件的监听者 可以实现血量变化的UI显示或者播放音效等
/*
 * 举个例子来说 当生命值发生变化时
 * AttributeSet 负责保存 Health = 75.f;
 * OverlayWidgetController 负责广播
 * OverlayWidget 负责显示
 * 那么这么说 游戏UI的控制流程就是 AbilitySystem->AttributeSet(Health MaxHealth)->WidgetController->Broadcast->Blueprint->Widget->UI
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

/**
 * 这个是专门负责主界面
 */
UCLASS(BlueprintType, Blueprintable)
class TOPDOWNVIEWDEMO_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	//通过广播来初始化角色HUD的函数
	virtual void BroadcastInitialValues() override;
	
	//绑定回调函数依赖的数据来源 我要去监听AbilitySystemComponent中的 Health 和 MaxHealth 的变化
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")//BlueprintAssignable 允许蓝图对于这个添加监听
	FOnHealthChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
};
