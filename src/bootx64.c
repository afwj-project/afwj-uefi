#include "eficio.h"

EFI_SYSTEM_TABLE* ST;
EFI_BOOT_SERVICES* BS;
EFI_RUNTIME_SERVICES* RT;

EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable) {
	EFI_STATUS Status;
	EFI_INPUT_KEY ShutdownKey;
	INTN DummyDecimal = -1024;
	gST = SystemTable;
	gBS = SystemTable->BootServices;
	gRT = SystemTable->RuntimeServices;
	Status = UefiInitializeConsole();
	if (Status != EFI_SUCCESS) return Status;
	gST->ConOut->OutputString(gST->ConOut, L"Initialized console and services.\r\n");
	UefiPrintDecimalInteger(DummyDecimal);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nPress keyboard to return.\r\n");
	do {
		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &ShutdownKey);
	} while (Status == EFI_NOT_READY);
	gST->ConOut->OutputString(gST->ConOut, L"c2p0ZntjMGxsMzkzXzBmX20zZDFjMW4zXzE1XzBwdGkwbjRsfQ==\r\n");
	gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
	return EFI_SUCCESS;
}
