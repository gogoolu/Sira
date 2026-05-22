#include "SiraGamePlay/BTTask_MeleeAttack.h"

#include "Enemy/NormalEnemy.h"
#include "Enemy/SiraAIController.h"

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	Handle = Enemy->OnMeleeAttackFinished.AddWeakLambda(this, [this, &Delegate = Enemy->OnMeleeAttackFinished]()
	{
		OnSkillFinished();
		Delegate.Remove(Handle);
	});
	
	Enemy->MeleeAttack();
	
	return EBTNodeResult::InProgress;
}

void UBTTask_MeleeAttack::OnSkillFinished() const
{
	if (CachedOwnerComp.IsValid())
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}
