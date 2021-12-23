#pragma once

#include "efierr.h"

VOID UefiFlushOutputBuffer();
VOID UefiInitializeConsole();
EFI_STATUS UefiPrintDecimalInteger(IN INT64 NumberData);
EFI_STATUS UefiPrintDecimalUnsigned(IN UINT64 NumberData);
EFI_STATUS UefiPrintHexadecimalUnsigned(IN UINT64 NumberData);
EFI_STATUS UefiPrintMemoryAddress(IN UINTN MemoryAddress);
EFI_STATUS UefiPrintProgressBar(IN UINTN StatusCount);
EFI_STATUS UefiScanSecretText(OUT CHAR16* StringBuffer, IN UINTN StringLength);
EFI_STATUS UefiScanPublicText(OUT CHAR16* StringBuffer, IN UINTN StringLength);
