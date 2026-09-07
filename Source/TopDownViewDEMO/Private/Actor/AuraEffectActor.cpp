
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
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();//记录上下文
	//给刚才这次 GameplayEffect 也就是游戏效果 的 Context 上下文 ，记录一下：这个效果的来源对象是 this 我是药水嘿嘿嘿 也就是说这一步是我在记录上下文
	EffectContextHandle.AddSourceObject(this);
	//获取一个可直接用于其他对象的输出游戏效果规范
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	//这个 ApplyGameplayEffectSpecToSelf 会返回一个句柄来着 就比如说如果一个技能需要创建一个活跃效果 然后小孩它所创建的那个特定效果 就必须通过此句柄、指针或活跃列表中的索引来实现
	//单纯的指针或索引是不够的 应为这些在客户端和服务器之间是不同步的
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Instant;
	
	if (bIsInfinite)
	{
		
	}
	
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
