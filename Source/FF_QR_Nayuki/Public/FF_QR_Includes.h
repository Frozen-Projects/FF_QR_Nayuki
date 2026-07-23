#pragma once

// Custom Includes.
#include "Nayuki_Enums.h"

// UE Includes
#include "ImageUtils.h"                         // Resize Nayuki QR

THIRD_PARTY_INCLUDES_START

// Nayuki QR.
#include "qrcodegen.hpp"                        // https://github.com/nayuki/QR-Code-generator

// ZXing.
#include "ReadBarcode.h"
#include "GTIN.h"
#include "MultiFormatWriter.h"
#include "BitMatrix.h"

#ifdef _WIN64

#include "Windows/AllowWindowsPlatformTypes.h"

// Ocean
#include "ocean/base/Frame.h"
#include "ocean/cv/FrameConverter.h"
#include "ocean/cv/FrameConverterBGRA32.h"
#include "ocean/cv/detector/qrcodes/QRCodeDetector2D.h"

#include "Windows/HideWindowsPlatformTypes.h"

#endif

THIRD_PARTY_INCLUDES_END