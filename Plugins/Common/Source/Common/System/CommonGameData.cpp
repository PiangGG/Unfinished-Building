#include "CommonGameData.h"

#include "CommonAssetManager.h"

UCommonGameData::UCommonGameData()
{
}

const UCommonGameData& UCommonGameData::Get()
{
	return UCommonAssetManager::Get().GetGameData();
}
