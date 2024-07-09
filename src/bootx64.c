#include "efi/eficio.h"
#include "efi/efimem.h"
#include "efi/efiutils.h"
#include "gdt.h"
#include "kbinfm.h"
#include "snailfs.h"

#define SYSTEM_CHECK_LOCATION { L"efi", L"afwjos", L"scheck.efi" }

EFI_HANDLE SystemCheckImage = NULL;

VOID ___chkstk_ms(){
	return;
}

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

EFI_STATUS UefiCheckOsbiSharedAddress(IN EFI_BLOCK_IO_PROTOCOL* sOperatingSystemBlockIo) {
	UINT8* CheckBuffer = (UINT8*)sOperatingSystemBlockIo;
	BOOLEAN DataFlag = FALSE;
	if (sOperatingSystemBlockIo != OSBI_SHARED_ADDRESS) return EFI_INVALID_PARAMETER;
	for (UINT8 i = 0; i < sizeof(EFI_BLOCK_IO_PROTOCOL); i++) {
		if (i % 16 == 0) gST->ConOut->OutputString(gST->ConOut, L"\r\n");
		UefiPrintHexadecimalUnsigned(CheckBuffer[i]);
		gST->ConOut->OutputString(gST->ConOut, L" ");
		if (CheckBuffer[i] && !DataFlag) DataFlag = TRUE;
	}
	gST->ConOut->OutputString(gST->ConOut, L"\r\n");
	if (DataFlag) return EFI_SUCCESS;
	else return EFI_NOT_FOUND;
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
	EFI_BLOCK_IO_PROTOCOL* sOperatingSystemBlockIo = OSBI_SHARED_ADDRESS;
	gST->ConOut->OutputString(gST->ConOut, L"Checking shared OS block I/O...\r\n");
	Status = UefiCheckOsbiSharedAddress(sOperatingSystemBlockIo);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"UefiCheckOsbiSharedAddress KOglKHh13wzf");
	// Unused codes is oFGs2zDOwEu9 HWxOnP5I4Rki tTmELBt5ii1W 0yeYNwaiUTEG
	SetupGlobalDescriptorTable();
	EFI_MEMORY_DESCRIPTOR* DescriptorEntry = NULL;
	MemoryMap.MemoryMapSize = EFI_MEMORY_MAP_SIZE;
	gBS->GetMemoryMap(
		&MemoryMap.MemoryMapSize, MemoryMap.MemoryMapBuffer, &MemoryMap.MapKey,
		&MemoryMap.DescriptorSize, &MemoryMap.DescriptorVersion);
	UINT64 BestAllocStart = 0;
	UINT64 BestNumberOfPages = 0;
	for (UINTN i = 0; i < MemoryMap.MemoryMapSize; i += MemoryMap.DescriptorSize) {
		DescriptorEntry = (EFI_MEMORY_DESCRIPTOR*)&MemoryMap.MemoryMapBuffer[i];
		if (DescriptorEntry->Type != EfiConventionalMemory) continue;
		if (DescriptorEntry->NumberOfPages > BestNumberOfPages) {
			BestNumberOfPages = DescriptorEntry->NumberOfPages;
			BestAllocStart = DescriptorEntry->PhysicalStart;
		}
	}
	NextAllocPage = BestAllocStart;
	SNAILFS_FILE* KernelFile = UefiSnailFileOpen(L"/kernel.bin", 'r', &Status);
	SectionInfo = (KERNEL_BINARY_SECTION_INFO*)UefiMalloc(sizeof(KERNEL_BINARY_SECTION_INFO) * 16);
	if (SectionInfo == NULL) UefiErrorShutdown(Status, L"UefiMalloc qkMTHm0FNbCw");
	gST->ConOut->OutputString(gST->ConOut, L"Reading kernel file...\r\n");
	KernelBuffer = (UINT8*)UefiMalloc(KernelFile->FileSize);
	UefiSnailFileRead(KernelBuffer, KernelFile->FileSize, KernelFile);
	UefiSnailFileClose(KernelFile);
	// TODO: add kernel file execution code
	UefiFree(SectionInfo);
	gST->ConOut->OutputString(gST->ConOut, L"Press keyboard to return.\r\n");
	do {
		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &ShutdownKey);
	} while (Status == EFI_NOT_READY);
	gST->ConOut->OutputString(gST->ConOut, L"Un escargot au lieu d\'une petite amie.\r\n");
	gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
	return EFI_SUCCESS;
}
