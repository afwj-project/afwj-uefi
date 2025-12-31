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

EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable) {
	EFI_STATUS Status;
	EFI_INPUT_KEY ShutdownKey;
	UINTN HandleCount;
	UINTN ExitDataSize = 0;
	CHAR16* ExitData = NULL;
	EFI_HANDLE* BlockControllerHandles = NULL;
	UINT8* BootRecordBuffer;
	UINT8* TableHdrBuffer;
	UINT8* GptHeaderBuffer = NULL;
	UINT8* PartitionEntryBuffer = NULL;
	EFI_PARTITION_TABLE_HEADER* GptHeader;
	EFI_PARTITION_ENTRY* PartitionEntry;
	FOUND_KERNEL_INFO* KernelInfo;
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
	Status = gBS->StartImage(SystemCheckImage, &ExitDataSize, &ExitData);
	if (Status != UD_EFI_FOUND) UefiErrorShutdown(Status, L"StartImage IBFivywzWeBZ");
	KernelInfo = (FOUND_KERNEL_INFO*)ExitData;
	gST->ConOut->OutputString(gST->ConOut, L"EXIT DATA SIZE: ");
	UefiPrintDecimalUnsigned(ExitDataSize);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nFOUND HANDLE INDEX: ");
	UefiPrintDecimalUnsigned(KernelInfo->FoundHandleIndex);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nChecking kernel binary code...\r\n");
	if (IsZeroSpace(KernelInfo->KernelBuffer.StartCode, KERNEL_START_CODE_SIZE))
		UefiErrorShutdown(Status, L"Kernel start code is empty! oFGs2zDOwEu9");
	Status = gBS->LocateHandleBuffer(ByProtocol, &gBlockIoProtocolGuid, NULL, &HandleCount, &BlockControllerHandles);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"LocateHandleBuffer KOglKHh13wzf");
	// Unused codes is HWxOnP5I4Rki tTmELBt5ii1W
	gST->ConOut->OutputString(gST->ConOut, L"Loading memory descriptor...\r\n");
	EFI_MEMORY_DESCRIPTOR* DescriptorEntry = NULL;
	MemoryMap.MemoryMapSize = EFI_MEMORY_MAP_SIZE;
	gBS->GetMemoryMap(
		&MemoryMap.MemoryMapSize, (EFI_MEMORY_DESCRIPTOR*)MemoryMap.MemoryMapBuffer,
		&MemoryMap.MapKey, &MemoryMap.DescriptorSize, &MemoryMap.DescriptorVersion);
	if (Status != EFI_SUCCESS) UefiErrorShutdown(Status, L"GetMemoryMap qkMTHm0FNbCw");
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
	gST->ConOut->OutputString(gST->ConOut, L"PAGE ADDRESS START: ");
	UefiPrintMemoryAddress(NextAllocPage);
	gST->ConOut->OutputString(gST->ConOut, L"\r\nPress keyboard to continue.\r\n");
	do {
		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &ShutdownKey);
	} while (Status == EFI_NOT_READY);
	gST->ConOut->OutputString(gST->ConOut, L"Loading global descriptor table...\r\n");
	gBS->ExitBootServices(ImageHandle, MemoryMap.MapKey);
	SetupGlobalDescriptorTable();
	SetupPaging();
	EnableSce();
	// TODO: add kernel file execution code
	gST->ConOut->OutputString(gST->ConOut, L"Press keyboard to shutdown.\r\n");
	do {
		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &ShutdownKey);
	} while (Status == EFI_NOT_READY);
	gST->ConOut->OutputString(gST->ConOut, L"Un escargot au lieu d\'une petite amie.\r\n");
	gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
	return EFI_SUCCESS;
}
