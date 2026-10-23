// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

/*
 * 火球命中敌人，调用应用 Gameplay Effect 的代码
 * GAS 执行 GE_Damage
 * GAS 发现它配置了 ExecCalc_Damage，于是调用 Execute_Implementation()
 * 执行代码，读取护甲、计算数值，并通过 OutExecutionOutput 把修改结果交给 GAS
 * GAS 处理这些修改，应用到对应属性上
 */

//专门用来收纳伤害计算所需的属性捕获定义的小盒子
struct AuraDamageStatics
{
	/* 相当于
	* FProperty* ArmorProperty; 让我们找到 Armor 这个属性对应的信息，后面也能用它指定要修改哪个属性
	* FGameplayEffectAttributeCaptureDefinition ArmorDef; 保存属性捕获规则，告诉 GAS 要捕获哪个属性、从哪一方捕获等
	 */
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	//第一次需要时，创建一个 DStatics 对象；之后再需要时，就继续使用这个对象
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	//告诉 GAS 我的这套伤害计算需要读取受击者的 Armor，请把这个属性捕获定义登记进来
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//分别获取攻击者和被攻击者的 ASC
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	//分别获取到攻击者和被攻击者的实际的 AActor 
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	//把这次攻击的来源方和目标方的相关标签取出来，留给后面的属性计算使用
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	//这个变量用来装属性评估所需的信息
	//这里的评估，就是 GAS 根据捕获定义、当前属性数据和相关条件，计算出这次应该使用的属性数值或加成
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float Armor = 0.f;
	//AttemptCalculateCapturedAttributeMagnitude 让 GAS 根据之前登记的属性捕获定义，计算出 Armor 属性的数值，并把结果写进 Armor 这个变量
	// DamageStatics().ArmorDef 用之前登记好的护甲捕获规则  EvaluateParameters 使用刚才准备好的评估条件 Armor 把计算结果写进这个变量
	const bool bSuccess = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, Armor);
	Armor = FMath::Max<float>(0.f, Armor);
	++Armor;
	
	//把结果交给 GAS 对 Armor 属性施加一个加 Armor 的修改
	const FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmorProperty, EGameplayModOp::Additive, Armor);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
