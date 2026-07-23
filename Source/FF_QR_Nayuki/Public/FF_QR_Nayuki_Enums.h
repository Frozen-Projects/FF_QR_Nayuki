#pragma once

#include "CoreMinimal.h"
#include "FF_QR_Nayuki_Enums.generated.h"

UENUM(BlueprintType, Category = "Frozen Forest|QR|Nayuki")
enum class ENayukiQrTolerance : uint8
{
	Quartile	UMETA(DisplayName = "Quartile"),
	Low			UMETA(DisplayName = "Low"),
	Mid			UMETA(DisplayName = "Mid"),
	High		UMETA(DisplayName = "High"),
};
ENUM_CLASS_FLAGS(ENayukiQrTolerance)