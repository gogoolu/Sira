#include "Manager/SiraGameplayTags.h"


FGameplayTag FSiraGameplayTags::GetTag_LifeState_Dead()
{
	static FGameplayTag LifeState_Dead = FGameplayTag::RequestGameplayTag("LifeState.Dead");
	return LifeState_Dead;
}

