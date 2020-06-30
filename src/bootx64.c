#include "eficio.h"

EFI_SYSTEM_TABLE* ST;
EFI_BOOT_SERVICES* BS;
EFI_RUNTIME_SERVICES* RT;

EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable) {
	EFI_STATUS Status;
	EFI_INPUT_KEY ShutdownKey;
	CHAR16 SecretText[64];
	CHAR16 PublicText[64];
	gST = SystemTable;
	gBS = SystemTable->BootServices;
	gRT = SystemTable->RuntimeServices;
	Status = UefiInitializeConsole();
	if (Status != EFI_SUCCESS) return Status;
	gST->ConOut->OutputString(gST->ConOut, L"Initialized console and services.\n");
	gST->ConOut->OutputString(gST->ConOut, L"SECRET> ");
	UefiScanSecretText(SecretText, 8);
	gST->ConOut->OutputString(gST->ConOut, L"\n[SECRET] ");
	gST->ConOut->OutputString(gST->ConOut, SecretText);
	gST->ConOut->OutputString(gST->ConOut, L"\nPUBLIC> ");
	UefiScanPublicText(PublicText, 8);
	gST->ConOut->OutputString(gST->ConOut, L"\n[PUBLIC] ");
	gST->ConOut->OutputString(gST->ConOut, PublicText);
	gST->ConOut->OutputString(gST->ConOut, L"\nPress keyboard to return.\n");
	do {
		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &ShutdownKey);
	} while (Status == EFI_NOT_READY);
	gST->ConOut->OutputString(gST->ConOut, L"c2p0ZntjMGxsMzkzXzBmX20zZDFjMW4zXzE1XzBwdGkwbjRsfQ==\n");
	gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
	return EFI_SUCCESS;
}
