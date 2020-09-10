#pragma once

#include "efigvars.h"

EFI_STATUS UefiFlushOutputBuffer();
EFI_STATUS UefiInitializeConsole();
EFI_STATUS UefiPrintDecimalInteger(IN INT64 NumberData);
EFI_STATUS UefiPrintDecimalUnsigned(IN UINT64 NumberData);
EFI_STATUS UefiScanSecretText(OUT CHAR16* StringBuffer, IN UINTN StringLength);
EFI_STATUS UefiScanPublicText(OUT CHAR16* StringBuffer, IN UINTN StringLength);
