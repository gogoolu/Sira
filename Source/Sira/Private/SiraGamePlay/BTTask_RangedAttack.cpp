#include "SiraGamePlay/BTTask_RangedAttack.h"

#include "Enemy/NormalEnemy.h"
#include "Enemy/SiraAIController.h"

EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = Cast<ASiraAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}
	
	auto Enemy = Cast<ANormalEnemy>(AIController->GetPawn());
	if (!Enemy || Enemy->IsAttacking())
	{
		return EBTNodeResult::Failed;
	}
	CachedOwnerComp = MakeWeakObjectPtr(&OwnerComp);
	Handle = Enemy->OnRangedAttackFinished.AddWeakLambda(this, [this, &Delegate = Enemy->OnRangedAttackFinished]()
	{
		OnSkillFinished();
		Delegate.Remove(Handle);
	});
	
	Enemy->RangedAttack();
	
	return EBTNodeResult::InProgress;
}

void UBTTask_RangedAttack::OnSkillFinished() const
{
	if (CachedOwnerComp.IsValid())
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}
