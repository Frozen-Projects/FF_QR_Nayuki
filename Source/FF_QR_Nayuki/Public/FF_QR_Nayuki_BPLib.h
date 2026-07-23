#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "FF_QR_Nayuki_Includes.h"

#include "FF_QR_Nayuki_BPLib.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FDelegateQrEncode, bool, bIsSuccessfull, FString, Out_Code, UTexture2D*, Out_QR);

UCLASS()
class FF_QR_NAYUKI_API UFF_QR_Nayuki_BPLib : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Nayuki QR - Encode QR Code", Keywords = "generate, encode, qr, nayuki"), Category = "Frozen Forest|QR|Nayuki")
	static void NayukiQr_GenerateQRCode(FDelegateQrEncode DelegateTexture2D, const FString Text, FVector2D Resolution = FVector2D(512, 512), int32 Border = 0, ENayukiQrTolerance ErrorTolerance = ENayukiQrTolerance::High, FColor Color_BG = FColor::White, FColor Color_Pattern = FColor::Black);

};