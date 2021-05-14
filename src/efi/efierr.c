#include "efi/eficio.h"

VOID UefiErrorPrint(IN EFI_STATUS ErrorCode, IN CHAR16* ErrorSymbol) {
	EFI_STATUS Status;
	EFI_INPUT_KEY ShutdownKey;
	gST->ConOut->OutputString(gST->ConOut, L"Error occurred!\r\nSTATUS: ");
	UefiPrintHexadecimalUnsigned(ErrorCode);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nSYMBOL: ");
	gST->ConOut->OutputString(gST->ConOut, ErrorSymbol);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nPress keyboard to continue.\r\n");
	do {
		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &ShutdownKey);
	} while (Status == EFI_NOT_READY);
}

VOID UefiErrorShutdown(IN EFI_STATUS ErrorCode, IN CHAR16* ErrorSymbol) {
	EFI_STATUS Status;
	EFI_INPUT_KEY ShutdownKey;
	gST->ConOut->OutputString(gST->ConOut, L"Error occurred!\r\nSTATUS: ");
	UefiPrintHexadecimalUnsigned(ErrorCode);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nSYMBOL: ");
	gST->ConOut->OutputString(gST->ConOut, ErrorSymbol);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nPress keyboard to return.\r\n");
	do {
		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &ShutdownKey);
	} while (Status == EFI_NOT_READY);
	gST->ConOut->OutputString(gST->ConOut, L"c2p0ZntjMGxsMzkzXzBmX20zZDFjMW4zXzE1XzBwdGkwbjRsfQ==\r\n");
	gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
}
