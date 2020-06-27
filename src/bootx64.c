#include "efigvars.h"

EFI_SYSTEM_TABLE* ST;
EFI_BOOT_SERVICES* BS;
EFI_RUNTIME_SERVICES* RT;

EFI_STATUS UefiScanSecretText(OUT CHAR16* StringBuffer, IN UINTN StringCount) {
	EFI_STATUS Status;
	EFI_INPUT_KEY PushedKey;
	UINTN i;
	for (i = 0; i < StringCount; i++) {
		do {
			Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &PushedKey);
		} while (Status == EFI_NOT_READY);
		StringBuffer[i] = PushedKey.UnicodeChar;
	}
	StringBuffer[i] = L'\0';
	return EFI_SUCCESS;
}

EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable) {
	EFI_STATUS Status;
	EFI_INPUT_KEY ShutdownKey;
	CHAR16 SecretText[64];
	gST = SystemTable;
	gBS = SystemTable->BootServices;
	gRT = SystemTable->RuntimeServices;
	Status = gST->ConOut->Reset(gST->ConOut, FALSE);
	if (Status != EFI_SUCCESS) return Status;
	Status = gST->ConIn->Reset(gST->ConIn, FALSE);
	if (Status != EFI_SUCCESS) return Status;
	gST->ConOut->OutputString(gST->ConOut, L"Initialized console and services.\n");
	gST->ConOut->OutputString(gST->ConOut, L"SECRET> ");
	UefiScanSecretText(SecretText, 8);
	gST->ConOut->OutputString(gST->ConOut, L"\n[SECRET] ");
	gST->ConOut->OutputString(gST->ConOut, SecretText);
	gST->ConOut->OutputString(gST->ConOut, L"\n");
	gST->ConOut->OutputString(gST->ConOut, L"Press keyboard to return.\n");
	do {
		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &ShutdownKey);
	} while (Status == EFI_NOT_READY);
	gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
	return EFI_SUCCESS;
}
