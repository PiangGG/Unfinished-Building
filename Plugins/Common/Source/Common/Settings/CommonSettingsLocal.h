#pragma once

#include "GameFramework/GameUserSettings.h"


#include "CommonSettingsLocal.generated.h"

USTRUCT()
struct FCommonScalabilitySnapshot
{
	GENERATED_BODY()

	FCommonScalabilitySnapshot();

	Scalability::FQualityLevels Qualities;
	bool bActive = false;
	bool bHasOverrides = false;
};

/**
 * CommonSettingsLocal
 */
UCLASS()
class UCommonSettingsLocal  : public UGameUserSettings
{
	GENERATED_BODY()
public:
	UCommonSettingsLocal();
	
};
