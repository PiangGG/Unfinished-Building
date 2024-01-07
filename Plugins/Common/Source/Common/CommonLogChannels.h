#pragma once

#include "Logging/LogMacros.h"

class UObject;

COMMON_API DECLARE_LOG_CATEGORY_EXTERN(LogCommon, Log, All);
COMMON_API DECLARE_LOG_CATEGORY_EXTERN(LogCommonExperience, Log, All);
COMMON_API DECLARE_LOG_CATEGORY_EXTERN(LogCommonAbilitySystem, Log, All);
COMMON_API DECLARE_LOG_CATEGORY_EXTERN(LogCommonTeams, Log, All);

COMMON_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
