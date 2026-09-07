
#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	//从目标 Actor 身上找到它的 AbilitySystemComponent
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;
	
	check(GameplayEffectClass);
	//Context 就可以理解为一次攻击的背景故事 比如玩家收到30点伤害 那么Context就记录了是谁造成的 是什么技能造成的 是从哪里来的 在哪里发生的
	//Handle 就是我拿来访问/持有某一个东西的一个句柄 就像储物柜的钥匙 虽然不直接等于柜子里的东西 但是他能让我找到并访问管理这个东西
	//所以 总的来说 GameplayEffect 就是要发生什么效果 GameplayEffectContext就是这次效果发生的背景信息 也就是这次效果发生时的背景是什么 Handle就是管理并访问它的一个句柄 也就是用什么东西去持有 / 操作这个Context
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();//记录上下文？ 也许是吧？ 上下文是什么？ 能吃吗？
	//给刚才这次 GameplayEffect 的 Context，记录一下：这个效果的来源对象是 this 我是药水嘿嘿嘿
	EffectContextHandle.AddSourceObject(this);
	//获取一个可直接用于其他对象的输出游戏效果规范
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
}
