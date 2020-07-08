#include "eficio.h"

CHAR16 OutputBuffer[1024];
UINTN OutputLength;

EFI_STATUS UefiFlushOutputBuffer() {
	for (UINTN i = 0; i < 1024; i++) OutputBuffer[i] = L'\0';
	OutputLength = 0;
	return EFI_SUCCESS;
}

EFI_STATUS UefiInitializeConsole() {
	EFI_STATUS Status;
	Status = gST->ConOut->Reset(gST->ConOut, FALSE);
	if (Status != EFI_SUCCESS) return Status;
	Status = gST->ConIn->Reset(gST->ConIn, FALSE);
	if (Status != EFI_SUCCESS) return Status;
	UefiFlushOutputBuffer();
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

EFI_STATUS UefiScanPublicText(OUT CHAR16* StringBuffer, IN UINTN StringLength) {
	EFI_STATUS Status;
	EFI_INPUT_KEY PushedKey;
	CHAR16 KeyOutputBuffer[2];
	UINTN i;
	for (i = 0; i < StringLength; i++) {
		do {
			Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &PushedKey);
		} while (Status == EFI_NOT_READY);
		KeyOutputBuffer[0] = PushedKey.UnicodeChar;
		KeyOutputBuffer[1] = L'\0';
		StringBuffer[i] = PushedKey.UnicodeChar;
		gST->ConOut->OutputString(gST->ConOut, KeyOutputBuffer);
	}
	StringBuffer[i] = L'\0';
	return EFI_SUCCESS;
}
