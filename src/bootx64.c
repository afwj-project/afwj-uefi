#include "eficio.h"

EFI_SYSTEM_TABLE* ST;
EFI_BOOT_SERVICES* BS;
EFI_RUNTIME_SERVICES* RT;

EFI_GUID LoadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
EFI_GUID DevicePathToTextProtocolGuid = EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID;
EFI_GUID DevicePathFromTextProtocolGuid = EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID;

EFI_LOADED_IMAGE_PROTOCOL* LoadedImageProtocol = NULL;
EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* DevicePathToTextProtocol = NULL;
EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL* DevicePathFromTextProtocol = NULL;
EFI_LOADED_IMAGE_PROTOCOL* ProtocolFromBootImage = NULL;

VOID UefiInitializeImage(IN EFI_HANDLE ImageHandle) {
	EFI_STATUS Status;
	Status = gBS->LocateProtocol(&gDevicePathToTextProtocolGuid, NULL, (VOID**)&gDevicePathToTextProtocol);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status);
	Status = gBS->LocateProtocol(&gDevicePathFromTextProtocolGuid, NULL, (VOID**)&gDevicePathFromTextProtocol);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status);
	Status = gBS->HandleProtocol(ImageHandle, &gLoadedImageProtocolGuid, (VOID**)&ProtocolFromBootImage);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status);
}

EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable) {
	EFI_STATUS Status;
	EFI_INPUT_KEY ShutdownKey;
	gST = SystemTable;
	gBS = SystemTable->BootServices;
	gRT = SystemTable->RuntimeServices;
	Status = gBS->SetWatchdogTimer(0, 0, 0, NULL);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status);
	UefiInitializeConsole();
	gST->ConOut->OutputString(gST->ConOut, L"Initialized console and services.\r\n");
	UefiInitializeImage(ImageHandle);
	gST->ConOut->OutputString(gST->ConOut, L"Initialized image and device path.\r\nVENDOR: ");
	gST->ConOut->OutputString(gST->ConOut, gST->FirmwareVendor);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nBASE: ");
	UefiPrintMemoryAddress((UINTN)ProtocolFromBootImage->ImageBase);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nSIZE: ");
	UefiPrintDecimalUnsigned(ProtocolFromBootImage->ImageSize);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nPATH: ");
	gST->ConOut->OutputString(
		gST->ConOut, gDevicePathToTextProtocol->ConvertDevicePathToText(
			ProtocolFromBootImage->FilePath, FALSE, FALSE));
	gST->ConOut->OutputString(gST->ConOut, L"\r\nPress keyboard to return.\r\n");
	do {
		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &ShutdownKey);
	} while (Status == EFI_NOT_READY);
	gST->ConOut->OutputString(gST->ConOut, L"c2p0ZntjMGxsMzkzXzBmX20zZDFjMW4zXzE1XzBwdGkwbjRsfQ==\r\n");
	gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
	return EFI_SUCCESS;
}
