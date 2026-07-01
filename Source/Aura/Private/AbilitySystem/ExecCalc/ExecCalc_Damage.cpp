// Copyright CRABOI


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	// 类似变量声明，此 declare 会产生一个 FProperty* 标识，和一个 FGameplayEffectAttributeCaptureDefinition 包装对象
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	AuraDamageStatics()
	{
		// 利用宏来创建属性捕获，与 MaxHealth 中的手动定义一样，更加 shortHand
		/*
		 * UAuraAttributeSet: 去哪个类捕获
		 * Armor : 属性名称
		 * Target / Source : 从目标或者源属性拿
		 * false: 是否启用快照
		 */
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , Armor , Target , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , BlockChance , Target , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , ArmorPenetration , Source , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , CriticalHitChance , Source , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , CriticalHitResistance , Target , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , CriticalHitDamage , Source , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , FireResistance , Target , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , LightningResistance , Target , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , ArcaneResistance , Target , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , PhysicalResistance , Target , false);

		const FAuraGameplayTags Tags = FAuraGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);

		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics; 
	return DStatics;
}
 
UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

// ExecutionParams：输入包：这次 GE 执行时，GAS 交给 ExecCalc 的全部上下文
// OutExecutionOutput：输出包：ExecCalc 算完后，把结果交回 GAS
void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	
	const UAbilitySystemComponent *SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent *TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor *SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor *TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	// 此处用于获取等级
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	// EvaluationParams 辅助属性计算的东西，和 ExecutionParams 庞大上下文分离，因为不同的 GA 的不同源目标，TAG 不同，计算规矩也不同。所以设计这个。
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	
	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances = FAuraGameplayTags::Get().DamageTypesToResistances;
	for (const auto& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTypeTag = Pair.Value;
		
		checkf(AuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceTypeTag), TEXT("TagsToCaptureDefs doesn't contain Tag:[%s] in ExecCalc_Damage"), *ResistanceTypeTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = AuraDamageStatics().TagsToCaptureDefs[ResistanceTypeTag];

		float DamageTypesValue = Spec.GetSetByCallerMagnitude(Pair.Key);
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParams,Resistance);
		Resistance = FMath::Clamp(Resistance,0.f,100.f);
		
		DamageTypesValue *= ( 100.f - Resistance ) / 100.f; 
		Damage += DamageTypesValue;
	}
	
	// Capture BlockChance on Target, and determine if there was a successful Block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParams,TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);

	const bool bBlocked = FMath::RandRange(1,100) <= TargetBlockChance;

	FGameplayEffectContextHandle EffectContextHandle =  Spec.GetContext();
	
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
	// If Blocked, halve the damage
	Damage = bBlocked ? Damage * 0.5f : Damage;
	
	
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParams,TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);
	
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParams,SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);
	
	
	const FRealCurve *ArmorPenetrationCurve =  CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	// ArmorPenetration ignores a percentage of the Target's Armor
	const float EffectiveArmor = TargetArmor * ( 100 - SourceArmorPenetration * ArmorPenetrationCoefficient ) / 100.f;

	const FRealCurve *EffectiveArmorCurve =  CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	// Armor ignores a percentage of incoming damage 
	Damage *= ( 100 - EffectiveArmor * EffectiveArmorCoefficient ) / 100.f;

	
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParams,SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParams,TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.f, TargetCriticalHitResistance);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParams,SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);

	
	const FRealCurve *CriticalResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	const float EffectiveCriticalHitChange = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	// Calculate weather it is a CriticalHit, CriticalHitResistance reduces CriticalHitChange by a certain percentage
	const bool bCriticalHit = FMath::RandRange(1, 100) <= EffectiveCriticalHitChange;

	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	// Double Damage plus a bonus if critical hit
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage  : Damage;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

