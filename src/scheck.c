#include "eficio.h"
#include "efimem.h"

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
	EFI_STATUS Status;
	UINTN HandleCount;
	EFI_HANDLE* BlockControllerHandles = NULL;
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
	gST->ConOut->OutputString(gST->ConOut, L"\r\nStarted to read blocks of hard drive.\r\n");
	Status = gBS->LocateHandleBuffer(ByProtocol, &gBlockIoProtocolGuid, NULL, &HandleCount, &BlockControllerHandles);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"LocateHandleBuffer xaCnH1jkgASt");
	UINT8* GptHeaderBuffer;
	EFI_PARTITION_TABLE_HEADER* GptHeader;
	EFI_DEVICE_PATH_PROTOCOL* BlockPath;
	CHAR16* BlockPathText;
	for (UINTN HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
		Status = gBS->HandleProtocol(BlockControllerHandles[HandleIndex], &gBlockIoProtocolGuid, (VOID**)&gBlockIoProtocol);
		if (Status != EFI_SUCCESS) continue;
		if (gBlockIoProtocol->Media->LogicalPartition) continue;
		Status = gBS->OpenProtocol(
			BlockControllerHandles[HandleIndex], &gDevicePathProtocolGuid, (VOID**)&BlockPath,
			ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
		if (Status != EFI_SUCCESS) continue;
		BlockPathText = gDevicePathToTextProtocol->ConvertDevicePathToText(BlockPath, TRUE, TRUE);
		gST->ConOut->OutputString(gST->ConOut, L"BLOCK");
		UefiPrintDecimalUnsigned(HandleIndex);
		gST->ConOut->OutputString(gST->ConOut, L": ");
		gST->ConOut->OutputString(gST->ConOut, BlockPathText);
		gST->ConOut->OutputString(gST->ConOut, L"\r\n");
		UefiFree(BlockPathText);
		GptHeaderBuffer = (UINT8*)UefiMalloc(gBlockIoProtocol->Media->BlockSize);
		Status = gBlockIoProtocol->ReadBlocks(
			gBlockIoProtocol, gBlockIoProtocol->Media->MediaId, (EFI_LBA)1,
			gBlockIoProtocol->Media->BlockSize, GptHeaderBuffer);
		GptHeader = (EFI_PARTITION_TABLE_HEADER*)GptHeaderBuffer;
		if (GptHeader->Hdr.Signature == EFI_PTAB_HEADER_ID) gST->ConOut->OutputString(gST->ConOut, L"GPT Header was found.\r\n");
		UefiFree(GptHeaderBuffer);
	}
	gST->ConOut->OutputString(gST->ConOut, L"Kernel is not ready.\r\n");
	return EFI_SUCCESS;
}
