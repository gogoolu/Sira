#pragma once


#include "BehaviorTree/Tasks/BTTask_MoveTo.h"

#include "BTTask_MeleeAttack.generated.h"

UCLASS()
class UBTTask_MeleeAttack : public UBTTask_BlackboardBase
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
