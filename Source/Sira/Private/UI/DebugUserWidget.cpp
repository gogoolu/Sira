#include "UI/DebugUserWidget.h"

void UDebugUserWidget::HandleCommand(const FString& Command)
{
	LastCommand = Command;
	// 如果函数存在，则通过 ProcessEvent 调用它
	if (UFunction* TargetFunc = FindFunction(*Command))
	{
		ProcessEvent(TargetFunc, nullptr);
	}
}
