#include "efi/eficio.h"
#include "efi/efimem.h"
#include "efi/efiutils.h"
#include "kbinfm.h"

#define SYSTEM_CHECK_LOCATION { L"efi", L"afwjos", L"scheck.efi" }

EFI_HANDLE SystemCheckImage = NULL;

VOID UefiInitializeImage(IN EFI_HANDLE ImageHandle) {
	EFI_STATUS Status;
	UefiMemInit();
	Status = gBS->LocateProtocol(&gDevicePathToTextProtocolGuid, NULL, (VOID**)&gDevicePathToTextProtocol);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"LocateProtocol Lc4bMjr5gxaW");
	Status = gBS->LocateProtocol(&gDevicePathFromTextProtocolGuid, NULL, (VOID**)&gDevicePathFromTextProtocol);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"LocateProtocol KMgqBBRJYLFN");
	Status = gBS->LocateProtocol(&gDevicePathUtilitiesProtocolGuid, NULL, (VOID**)&gDevicePathUtilitiesProtocol);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"LocateProtocol omELRBEyMb4a");
	Status = gBS->HandleProtocol(ImageHandle, &gLoadedImageProtocolGuid, (VOID**)&gLoadedImageProtocol);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"HandleProtocol FYJYiyWsbSbM");
}

VOID UefiLoadSystemCheck(IN EFI_HANDLE ImageHandle) {
	EFI_STATUS Status;
	CHAR16* SystemCheckLocation[3] = SYSTEM_CHECK_LOCATION;
	EFI_DEVICE_PATH_PROTOCOL* SystemCheckNodes[3];
	EFI_DEVICE_PATH_PROTOCOL* SystemCheckPath;
	for (UINTN i = 0; i < 3; i++) SystemCheckNodes[i] = gDevicePathFromTextProtocol->ConvertTextToDeviceNode(SystemCheckLocation[i]);
	Status = gBS->OpenProtocol(
		gLoadedImageProtocol->DeviceHandle, &gDevicePathProtocolGuid, (VOID**)&SystemCheckPath,
		ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"OpenProtocol 5roQjGJTiHQM");
	gST->ConOut->OutputString(gST->ConOut, L"\r\nDEVP: ");
	gST->ConOut->OutputString(
		gST->ConOut, gDevicePathToTextProtocol->ConvertDevicePathToText(
			SystemCheckPath, FALSE, FALSE));
	gST->ConOut->OutputString(gST->ConOut, L"\r\nFinding system check application... ");
	for (UINTN i = 0; i < 3; i++) SystemCheckPath = gDevicePathUtilitiesProtocol->AppendDeviceNode(SystemCheckPath, SystemCheckNodes[i]);
	Status = gBS->LoadImage(FALSE, ImageHandle, SystemCheckPath, NULL, 0, &SystemCheckImage);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"LoadImage sa5Q2t/ritW7");
}

EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable) {
	EFI_STATUS Status;
	EFI_INPUT_KEY ShutdownKey;
	UINTN HandleCount;
	EFI_HANDLE* BlockControllerHandles = NULL;
	UINT8* BootRecordBuffer;
	UINT8* TableHdrBuffer;
	UINT8* GptHeaderBuffer = NULL;
	UINT8* PartitionEntryBuffer = NULL;
	EFI_PARTITION_TABLE_HEADER* GptHeader;
	EFI_PARTITION_ENTRY* PartitionEntry;
	UINT8* KernelBuffer = NULL;
	KERNEL_BINARY_SECTION_INFO* SectionInfo;
	UINTN NumberOfEntryBlocks;
	BOOLEAN FoundFlag = FALSE;
	UINT64 EntryReadingControl = 0;
	gST = SystemTable;
	gBS = SystemTable->BootServices;
	gRT = SystemTable->RuntimeServices;
	Status = gBS->SetWatchdogTimer(0, 0, 0, NULL);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"SetWatchdogTimer LVRgc+p7CSoc");
	UefiInitializeConsole();
	gST->ConOut->OutputString(gST->ConOut, L"Hello, world! This is Achatina Fulica White Jade.\r\n");
	gST->ConOut->OutputString(gST->ConOut, L"Initialized console and services.\r\n");
	UefiInitializeImage(ImageHandle);
	gST->ConOut->OutputString(gST->ConOut, L"Initialized image and device path.\r\nVENDOR: ");
	gST->ConOut->OutputString(gST->ConOut, gST->FirmwareVendor);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nBASE: ");
	UefiPrintMemoryAddress((UINTN)gLoadedImageProtocol->ImageBase);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nSIZE: ");
	UefiPrintDecimalUnsigned(gLoadedImageProtocol->ImageSize);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nPATH: ");
	gST->ConOut->OutputString(
		gST->ConOut, gDevicePathToTextProtocol->ConvertDevicePathToText(
			gLoadedImageProtocol->FilePath, FALSE, FALSE));
	UefiLoadSystemCheck(ImageHandle);
	gST->ConOut->OutputString(gST->ConOut, L"PROFIT!\r\nRunning system check application...\r\n");
	Status = gBS->StartImage(SystemCheckImage, NULL, NULL);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"StartImage IBFivywzWeBZ");
	Status = gBS->LocateHandleBuffer(ByProtocol, &gBlockIoProtocolGuid, NULL, &HandleCount, &BlockControllerHandles);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"LocateHandleBuffer oFGs2zDOwEu9");
	for (UINTN HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
		Status = gBS->HandleProtocol(BlockControllerHandles[HandleIndex], &gBlockIoProtocolGuid, (VOID**)&gBlockIoProtocol);
		if (Status != EFI_SUCCESS) {
			UefiErrorPrint(Status, L"HandleProtocol HWxOnP5I4Rki");
			continue;
		}
		if (gBlockIoProtocol->Media->LogicalPartition) continue;
		GptHeaderBuffer = (UINT8*)UefiMalloc(gBlockIoProtocol->Media->BlockSize);
		Status = gBlockIoProtocol->ReadBlocks(
			gBlockIoProtocol, gBlockIoProtocol->Media->MediaId, (EFI_LBA)1,
			gBlockIoProtocol->Media->BlockSize, GptHeaderBuffer);
		if (Status != EFI_SUCCESS) {
			UefiFree(GptHeaderBuffer);
			UefiErrorPrint(Status, L"ReadBlocks tTmELBt5ii1W");
			continue;
		}
		GptHeader = (EFI_PARTITION_TABLE_HEADER*)GptHeaderBuffer;
		if (GptHeader->Hdr.Signature != EFI_PTAB_HEADER_ID) {
			UefiFree(GptHeaderBuffer);
			continue;
		}
		NumberOfEntryBlocks = GptHeader->NumberOfPartitionEntries / 4;
		for (UINTN EntryBlockIndex = 0; EntryBlockIndex < NumberOfEntryBlocks; EntryBlockIndex++) {
			PartitionEntryBuffer = (UINT8*)UefiMalloc(gBlockIoProtocol->Media->BlockSize);
			Status = gBlockIoProtocol->ReadBlocks(
				gBlockIoProtocol, gBlockIoProtocol->Media->MediaId, (EFI_LBA)(2 + EntryBlockIndex),
				gBlockIoProtocol->Media->BlockSize, PartitionEntryBuffer);
			if (Status != EFI_SUCCESS) {
				UefiFree(PartitionEntryBuffer);
				UefiErrorPrint(Status, L"ReadBlocks 0yeYNwaiUTEG");
				continue;
			}
			PartitionEntry = (EFI_PARTITION_ENTRY*)PartitionEntryBuffer;
			for (UINTN EntryDataIndex = 0; EntryDataIndex < 4; EntryDataIndex++) {
				EntryReadingControl += (UINT64)IsZeroGUID(PartitionEntry[EntryDataIndex].PartitionTypeGUID);
				EntryReadingControl += (UINT64)(EntryBlockIndex * 4 + EntryDataIndex != 1) << 1;
				EntryReadingControl += (UINT64)FoundFlag << 2;
				if (EntryReadingControl > 0) {
					EntryReadingControl = 0;
					continue;
				}
				UefiMemCpy(gOperatingSystemEntry, &PartitionEntry[EntryDataIndex], sizeof(EFI_PARTITION_ENTRY));
			}
			UefiFree(PartitionEntryBuffer);
		}
		UefiFree(GptHeaderBuffer);
		if (!FoundFlag) {
			gOperatingSystemBlockIo = gBlockIoProtocol;
			FoundFlag = TRUE;
		}
	}
	SectionInfo = (KERNEL_BINARY_SECTION_INFO*)UefiMalloc(sizeof(KERNEL_BINARY_SECTION_INFO) * 16);
	if (SectionInfo == NULL) UefiErrorShutdown(Status, L"UefiMalloc qkMTHm0FNbCw");
	gST->ConOut->OutputString(gST->ConOut, L"Reading kernel file...\r\n");
	// TODO: add kernel file execution code
	UefiFree(SectionInfo);
	gST->ConOut->OutputString(gST->ConOut, L"Press keyboard to return.\r\n");
	do {
		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &ShutdownKey);
	} while (Status == EFI_NOT_READY);
	gST->ConOut->OutputString(gST->ConOut, L"Good bye, big friend who likes ScTechEngiAMath!\r\n");
	gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
	return EFI_SUCCESS;
}
