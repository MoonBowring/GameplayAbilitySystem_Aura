#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

//这个是自己写的一个 FGameplayEffectContext 主要是想在继承原来的功能上 再增加暴击和格挡的信息 后面估计还会有功能
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	
	//当 FAuraGameplayEffectContext 对象通过父类 FGameplayEffectContext 指针使用时
	//调用 GetScriptStruct()，我们希望返回的是子类 FAuraGameplayEffectContext 的类型信息，而不是父类的类型信息
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}
	
	//创建一个新的 FAuraGameplayEffectContext 对象
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		//复制整个 Context，然后特别把 HitResult 单独复制一份，避免两个 Context 共用同一份命中数据
		//原因在于 HitResult 在父类中不是直接保存的 而是通过智能指针 TSharedPtr 保存的
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	//负责规定自定义 Context 中的数据，应该如何进行网络序列化和反序列化
	//不是让整个 Context 不分情况地一直复制到所有客户端，而是在 GAS 需要通过网络传输这份 Context 的场景中，负责处理其中的数据
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
protected:
	
	//这个布尔值是用来保存这次攻击 也就是 FGameplayEffectContext 是否格挡的
	UPROPERTY()
	bool bIsBlockedHit = false;
	
	//这个布尔值是用来保存这次攻击是否保暴击的
	UPROPERTY()
	bool bIsCriticalHit = false;
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		//结构体有自己的网络序列化函数 请使用自己写的 NetSerialize()
		WithNetSerializer = true,
		//结构体支持自定义的复制操作 可以使用对应的复制逻辑
		WithCopy = true
	};
};
