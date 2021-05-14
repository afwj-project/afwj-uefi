#include "efi/eficio.h"

CHAR16 OutputBuffer[1024];
UINTN OutputLength;

VOID UefiFlushOutputBuffer() {
	for (UINTN i = 0; i < 1024; i++) OutputBuffer[i] = L'\0';
	OutputLength = 0;
}

VOID UefiInitializeConsole() {
	EFI_STATUS Status;
	Status = gST->ConOut->Reset(gST->ConOut, FALSE);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"ConOut::Reset WW92chBGtZKQ");
	Status = gST->ConIn->Reset(gST->ConIn, FALSE);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"ConIn::Reset ksThRY1Ii2d3");
	UefiFlushOutputBuffer();
}

EFI_STATUS UefiPrintDecimalInteger(IN INT64 NumberData) {
	BOOLEAN IsMinus = NumberData < 0;
	CHAR16 TempCharacter;
	if (IsMinus) NumberData = -NumberData;
	do {
		OutputBuffer[OutputLength++] = L'0' + NumberData % 10;
		NumberData /= 10;
	} while (NumberData);
	if (IsMinus) OutputBuffer[OutputLength++] = L'-';
	UINTN LastIndex = OutputLength - 1;
	for (UINTN i = 0; i < OutputLength / 2; i++) {
		TempCharacter = OutputBuffer[i];
		OutputBuffer[i] = OutputBuffer[LastIndex - i];
		OutputBuffer[LastIndex - i] = TempCharacter;
	}
	gST->ConOut->OutputString(gST->ConOut, OutputBuffer);
	UefiFlushOutputBuffer();
	return EFI_SUCCESS;
}

EFI_STATUS UefiPrintDecimalUnsigned(IN UINT64 NumberData) {
	CHAR16 TempCharacter;
	do {
		OutputBuffer[OutputLength++] = L'0' + NumberData % 10;
		NumberData /= 10;
	} while (NumberData);
	UINTN LastIndex = OutputLength - 1;
	for (UINTN i = 0; i < OutputLength / 2; i++) {
		TempCharacter = OutputBuffer[i];
		OutputBuffer[i] = OutputBuffer[LastIndex - i];
		OutputBuffer[LastIndex - i] = TempCharacter;
	}
	gST->ConOut->OutputString(gST->ConOut, OutputBuffer);
	UefiFlushOutputBuffer();
	return EFI_SUCCESS;
}

EFI_STATUS UefiPrintHexadecimalUnsigned(IN UINT64 NumberData) {
	UINTN RemainderData;
	CHAR16 TempCharacter;
	do {
		RemainderData = NumberData % 16;
		if (RemainderData < 10) OutputBuffer[OutputLength++] = L'0' + RemainderData;
		else OutputBuffer[OutputLength++] = L'7' + RemainderData;
		NumberData /= 16;
	} while (NumberData);
	UINTN LastIndex = OutputLength - 1;
	for (UINTN i = 0; i < OutputLength / 2; i++) {
		TempCharacter = OutputBuffer[i];
		OutputBuffer[i] = OutputBuffer[LastIndex - i];
		OutputBuffer[LastIndex - i] = TempCharacter;
	}
	gST->ConOut->OutputString(gST->ConOut, OutputBuffer);
	UefiFlushOutputBuffer();
	return EFI_SUCCESS;
}

EFI_STATUS UefiPrintMemoryAddress(IN UINTN MemoryAddress) {
	UINTN RemainderData;
	CHAR16 TempCharacter;
	for (UINTN i = 0; i < 16; i++) {
		RemainderData = MemoryAddress % 16;
		if (RemainderData < 10) OutputBuffer[OutputLength++] = L'0' + RemainderData;
		else OutputBuffer[OutputLength++] = L'7' + RemainderData;
		MemoryAddress /= 16;
	}
	UINTN LastIndex = OutputLength - 1;
	for (UINTN i = 0; i < OutputLength / 2; i++) {
		TempCharacter = OutputBuffer[i];
		OutputBuffer[i] = OutputBuffer[LastIndex - i];
		OutputBuffer[LastIndex - i] = TempCharacter;
	}
	gST->ConOut->OutputString(gST->ConOut, OutputBuffer);
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
