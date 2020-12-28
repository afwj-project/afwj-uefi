#include "eficio.h"

VOID UefiInitializeApplication(IN EFI_HANDLE ImageHandle) {
	EFI_STATUS Status;
	UefiFlushOutputBuffer();
	Status = gBS->LocateProtocol(&gDevicePathToTextProtocolGuid, NULL, (VOID**)&gDevicePathToTextProtocol);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"LocateProtocol Ym6Da/DYMiaJ");
	Status = gBS->LocateProtocol(&gDevicePathFromTextProtocolGuid, NULL, (VOID**)&gDevicePathFromTextProtocol);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"LocateProtocol GL9fznDL3R3v");
	Status = gBS->LocateProtocol(&gBlockIoProtocolGuid, NULL, (VOID**)&gBlockIoProtocol);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"LocateProtocol 0E0rv3MRTkTu");
	Status = gBS->HandleProtocol(ImageHandle, &gLoadedImageProtocolGuid, (VOID**)&gLoadedImageProtocol);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"HandleProtocol 3l4xR6bb1MpT");
}

EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable) {
	gST = SystemTable;
	gBS = SystemTable->BootServices;
	gRT = SystemTable->RuntimeServices;
	UefiInitializeApplication(ImageHandle);
	gST->ConOut->OutputString(gST->ConOut, L"Initialized system check application.\r\nBASE: ");
	UefiPrintMemoryAddress((UINTN)gLoadedImageProtocol->ImageBase);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nSIZE: ");
	UefiPrintDecimalUnsigned(gLoadedImageProtocol->ImageSize);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nPATH: ");
	gST->ConOut->OutputString(
		gST->ConOut, DevicePathToTextProtocol->ConvertDevicePathToText(
			gLoadedImageProtocol->FilePath, FALSE, FALSE));
	gST->ConOut->OutputString(gST->ConOut, L"\r\nKernel is not ready.\r\n");
	return EFI_SUCCESS;
}
