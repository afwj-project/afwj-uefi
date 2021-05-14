#include "efi/eficio.h"
#include "efi/efimem.h"
#include "kbinfm.h"

#define SYSTEM_CHECK_LOCATION { L"efi", L"afwjos", L"scheck.efi" }

EFI_HANDLE SystemCheckImage = NULL;

VOID UefiInitializeImage(IN EFI_HANDLE ImageHandle) {
	EFI_STATUS Status;
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
	KERNEL_BINARY_SECTION_INFO* SectionInfo;
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
	SectionInfo = (KERNEL_BINARY_SECTION_INFO*)UefiMalloc(sizeof(KERNEL_BINARY_SECTION_INFO) * 16);
	if (SectionInfo == NULL) UefiErrorShutdown(Status, L"UefiMalloc qkMTHm0FNbCw");
	UefiFree(SectionInfo);
	gST->ConOut->OutputString(gST->ConOut, L"Press keyboard to return.\r\n");
	do {
		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &ShutdownKey);
	} while (Status == EFI_NOT_READY);
	gST->ConOut->OutputString(gST->ConOut, L"SnVuLVRhZSBTb24KUmVwdWJsaWMgb2YgS29yZWEKU0pUIFNvZnR3YXJlIENvcnBvcmF0aW9u\r\n");
	gST->ConOut->OutputString(gST->ConOut, L"UmVwdWJsaWMgb2YgS29yZWEgQWlyIEZvcmNlIE9mZmljZXIgVHJhaW5pbmcgU2Nob29sIEV4YW1pbmF0aW9u\r\n");
	gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
	return EFI_SUCCESS;
}
