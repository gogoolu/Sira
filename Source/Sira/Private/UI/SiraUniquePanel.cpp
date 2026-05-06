#include "UI/SiraUniquePanel.h"

FName USiraUniquePanel::GetUIKey() const
{
	return UIKey;
}

void USiraUniquePanel::SetUIKey(FName Key)
{
	UIKey = Key;
}
