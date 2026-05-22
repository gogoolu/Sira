#pragma once
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"

#include "BTTask_RangedAttack.generated.h"

UCLASS()
class UBTTask_RangedAttack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
private:
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;
	
	
	void OnSkillFinished() const;
	
	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
	
	FDelegateHandle Handle;
};
