#include "eficio.h"

CHAR16 OutputBuffer[1024];

EFI_STATUS UefiInitializeConsole() {
	EFI_STATUS Status;
	Status = gST->ConOut->Reset(gST->ConOut, FALSE);
	if (Status != EFI_SUCCESS) return Status;
	Status = gST->ConIn->Reset(gST->ConIn, FALSE);
	if (Status != EFI_SUCCESS) return Status;
	for (UINTN i = 0; i < 1024; i++) OutputBuffer[i] = L'\0';
	return EFI_SUCCESS;
}

EFI_STATUS UefiScanSecretText(OUT CHAR16* StringBuffer, IN UINTN StringLength) {
	EFI_STATUS Status;
	EFI_INPUT_KEY PushedKey;
	UINTN i;
	for (i = 0; i < StringLength; i++) {
		do {
			Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &PushedKey);
		} while (Status == EFI_NOT_READY);
		StringBuffer[i] = PushedKey.UnicodeChar;
	}
	StringBuffer[i] = L'\0';
	return EFI_SUCCESS;
}
