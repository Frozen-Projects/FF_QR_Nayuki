#include "FF_QR_Nayuki_BPLib.h"
#include "FF_QR_Nayuki.h"

UFF_QR_Nayuki_BPLib::UFF_QR_Nayuki_BPLib(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UFF_QR_Nayuki_BPLib::NayukiQr_GenerateQRCode(FDelegateQrEncode DelegateTexture2D, const FString In_Text, FVector2D Resolution, int32 Border, ENayukiQrTolerance ErrorTolerance, FColor Color_BG, FColor Color_Pattern)
{
    const int32 TextureWidth = FMath::RoundToInt(Resolution.X);
    const int32 TextureHeight = FMath::RoundToInt(Resolution.Y);
    const int32 PixelBorder = FMath::Max(Border, 0);

    if (In_Text.IsEmpty())
    {
        DelegateTexture2D.ExecuteIfBound(false, "Nayuki QR encode failed. Input text is empty.", nullptr);
        return;
    }

    if (TextureWidth <= 0 || TextureHeight <= 0)
    {
        DelegateTexture2D.ExecuteIfBound(false, "Nayuki QR encode failed. Resolution must be greater than zero.", nullptr);
        return;
    }

    if (TextureWidth <= PixelBorder * 2 || TextureHeight <= PixelBorder * 2)
    {
        DelegateTexture2D.ExecuteIfBound(false, "Nayuki QR encode failed. Border is too large for the requested resolution.", nullptr);
        return;
    }

    Color_BG.A = 255;
    Color_Pattern.A = 255;

    AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [DelegateTexture2D, In_Text, TextureWidth, TextureHeight, PixelBorder, ErrorTolerance, Color_BG, Color_Pattern]()
        {
            qrcodegen::QrCode::Ecc ToleranceLevel = qrcodegen::QrCode::Ecc::LOW;

            switch (ErrorTolerance)
            {
                case ENayukiQrTolerance::Quartile:
                    ToleranceLevel = qrcodegen::QrCode::Ecc::QUARTILE;
                    break;

                case ENayukiQrTolerance::Low:
                    ToleranceLevel = qrcodegen::QrCode::Ecc::LOW;
                    break;

                case ENayukiQrTolerance::Mid:
                    ToleranceLevel = qrcodegen::QrCode::Ecc::MEDIUM;
                    break;

                case ENayukiQrTolerance::High:
                    ToleranceLevel = qrcodegen::QrCode::Ecc::HIGH;
                    break;

                default:
                    ToleranceLevel = qrcodegen::QrCode::Ecc::HIGH;
                    break;
            }

            const FTCHARToUTF8 UTF8Text(*In_Text);
            const qrcodegen::QrCode QRCode = qrcodegen::QrCode::encodeText(UTF8Text.Get(), ToleranceLevel);

            const int32 QRSize = QRCode.getSize();
            const int32 AvailableWidth = TextureWidth - PixelBorder * 2;
            const int32 AvailableHeight = TextureHeight - PixelBorder * 2;

            if (AvailableWidth < QRSize || AvailableHeight < QRSize)
            {
                AsyncTask(ENamedThreads::GameThread, [DelegateTexture2D, QRSize]()
                    {
                        DelegateTexture2D.ExecuteIfBound(
                            false,
                            FString::Printf(TEXT("Nayuki QR encode failed. Inner resolution must be at least %dx%d pixels."), QRSize, QRSize),
                            nullptr);
                    });

                return;
            }

            const int64 PixelCount64 = static_cast<int64>(TextureWidth) * static_cast<int64>(TextureHeight);

            if (PixelCount64 > MAX_int32)
            {
                AsyncTask(ENamedThreads::GameThread, [DelegateTexture2D]()
                    {
                        DelegateTexture2D.ExecuteIfBound(false, "Nayuki QR encode failed. Requested texture is too large.", nullptr);
                    });

                return;
            }

            TArray<FColor> FinalPixels;
            FinalPixels.Init(Color_BG, static_cast<int32>(PixelCount64));

            for (int32 ModuleY = 0; ModuleY < QRSize; ++ModuleY)
            {
                const int32 StartY = PixelBorder + ModuleY * AvailableHeight / QRSize;
                const int32 EndY = PixelBorder + (ModuleY + 1) * AvailableHeight / QRSize;

                for (int32 ModuleX = 0; ModuleX < QRSize; ++ModuleX)
                {
                    if (!QRCode.getModule(ModuleX, ModuleY))
                    {
                        continue;
                    }

                    const int32 StartX = PixelBorder + ModuleX * AvailableWidth / QRSize;
                    const int32 EndX = PixelBorder + (ModuleX + 1) * AvailableWidth / QRSize;
                    const int32 ModulePixelWidth = EndX - StartX;

                    for (int32 PixelY = StartY; PixelY < EndY; ++PixelY)
                    {
                        FColor* Row = FinalPixels.GetData() + PixelY * TextureWidth + StartX;

                        for (int32 PixelX = 0; PixelX < ModulePixelWidth; ++PixelX)
                        {
                            Row[PixelX] = Color_Pattern;
                        }
                    }
                }
            }

            AsyncTask(ENamedThreads::GameThread, [DelegateTexture2D, TextureWidth, TextureHeight, FinalPixels = MoveTemp(FinalPixels)]()
                {
                    UTexture2D* Texture = UTexture2D::CreateTransient(TextureWidth, TextureHeight, PF_B8G8R8A8);

                    if (!IsValid(Texture) || !Texture->GetPlatformData() || Texture->GetPlatformData()->Mips.IsEmpty())
                    {
                        DelegateTexture2D.ExecuteIfBound(false, "Nayuki QR encode failed. Texture creation failed.", nullptr);
                        return;
                    }

                    Texture->Filter = TextureFilter::TF_Nearest;
                    Texture->AddressX = TextureAddress::TA_Clamp;
                    Texture->AddressY = TextureAddress::TA_Clamp;
                    Texture->SRGB = true;
                    Texture->NeverStream = true;
                    Texture->LODGroup = TextureGroup::TEXTUREGROUP_Pixels2D;

                    FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
                    void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);

                    if (!TextureData)
                    {
                        Mip.BulkData.Unlock();
                        DelegateTexture2D.ExecuteIfBound(false, "Nayuki QR encode failed. Texture memory could not be locked.", nullptr);
                        return;
                    }

                    FMemory::Memcpy(TextureData, FinalPixels.GetData(), FinalPixels.Num() * sizeof(FColor));
                    Mip.BulkData.Unlock();

                    Texture->UpdateResource();

                    DelegateTexture2D.ExecuteIfBound(true, "Nayuki QR encode is successful.", Texture);
                });
        });
}