#pragma once

#include "efigvars.h"

#define EFI_SUCCESS 0
#define EFI_LOAD_ERROR ((EFI_STATUS)(MAX_BIT | 0x1))
#define EFI_INVALID_PARAMETER ((EFI_STATUS)(MAX_BIT | 0x2))
#define EFI_UNSUPPORTED ((EFI_STATUS)(MAX_BIT | 0x3))
#define EFI_NOT_READY ((EFI_STATUS)(MAX_BIT | 0x6))
#define EFI_DEVICE_ERROR ((EFI_STATUS)(MAX_BIT | 0x7))
#define EFI_NO_MEDIA ((EFI_STATUS)(MAX_BIT | 0xC))
#define EFI_NOT_FOUND ((EFI_STATUS)(MAX_BIT | 0xE))

VOID UefiErrorPrint(IN EFI_STATUS ErrorCode, IN CHAR16* ErrorSymbol);
VOID UefiErrorShutdown(IN EFI_STATUS ErrorCode, IN CHAR16* ErrorSymbol);
