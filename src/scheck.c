#include "eficio.h"
#include "efimem.h"
#include "efiutils.h"
#include "snailfs.h"

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

VOID UefiCheckSnailBootRecord(IN SNAILFS_BOOT_RECORD* BootRecord, OUT EFI_STATUS* StatusRef) {
	if (BootRecord->Signature != SNAILFS_BOOT_SIGNATURE) {
		gST->ConOut->OutputString(gST->ConOut, L"Boot signature was not found.\r\n");
		*StatusRef = EFI_NOT_FOUND;
	} else {
		gST->ConOut->OutputString(gST->ConOut, L"Boot signature was found.\r\n");
		*StatusRef = EFI_SUCCESS;
	}
}

VOID UefiCheckSnailTableHdr(IN SNAILFS_TABLE_HEADER* TableHdr, OUT EFI_STATUS* StatusRef) {
	if (TableHdr->FileSystemChecking != SNAILFS_MAGIC_CODE) {
		gST->ConOut->OutputString(gST->ConOut, L"Unsupported file system.\r\n");
		goto COMPATIBILITY_ERROR;
	}
	if (TableHdr->VersionInformation != SNAILFS_0_10_REVISION) {
		gST->ConOut->OutputString(gST->ConOut, L"Version of file system is not compatible with operating system.\r\n");
		goto COMPATIBILITY_ERROR;
	}
	if (TableHdr->OperatingSystemChecking != SNAILFS_AFWJOS_CHECK) {
		gST->ConOut->OutputString(gST->ConOut, L"Failed to check operating system.\r\n");
		goto COMPATIBILITY_ERROR;
	}
	gST->ConOut->OutputString(gST->ConOut, L"Supported file system was found.\r\n");
	goto CHECKING_PROFIT;
COMPATIBILITY_ERROR:
	*StatusRef = EFI_UNSUPPORTED;
	return;
CHECKING_PROFIT:
	*StatusRef = EFI_SUCCESS;
}

VOID UefiCheckSnailTableSize(
	IN SNAILFS_TABLE_HEADER* TableHdr,
	IN EFI_PARTITION_ENTRY* PartitionEntry,
	OUT EFI_STATUS* StatusRef
) {
	UINT64 FromEntry = PartitionEntry->EndingLBA - PartitionEntry->StartingLBA + 1;
	UINT64 FromHdr = 2 + TableHdr->MaximumTableLength * 8 + TableHdr->MaximumTotalBlocks;
	if (FromEntry != FromHdr) {
		gST->ConOut->OutputString(gST->ConOut, L"Failed to check size of partition.\r\n");
		goto INVALID_HEADER;
	}
	if (TableHdr->EndingDataAddress - TableHdr->StartingDataAddress + 1 != TableHdr->MaximumTotalBlocks) {
		gST->ConOut->OutputString(gST->ConOut, L"Failed to check size of data area.\r\n");
		goto INVALID_HEADER;
	}
	gST->ConOut->OutputString(gST->ConOut, L"Table header is normal.\r\n");
	goto CHECKING_PROFIT;
INVALID_HEADER:
	*StatusRef = EFI_DEVICE_ERROR;
	return;
CHECKING_PROFIT:
	*StatusRef = EFI_SUCCESS;
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
	EFI_PARTITION_ENTRY* OperatingSystemEntry = (EFI_PARTITION_ENTRY*)UefiMalloc(sizeof(EFI_PARTITION_ENTRY));
	UINT8* BootRecordBuffer;
	UINT8* TableHdrBuffer;
	UINT8* GptHeaderBuffer;
	UINT8* PartitionEntryBuffer;
	EFI_PARTITION_TABLE_HEADER* GptHeader;
	EFI_PARTITION_ENTRY* PartitionEntry;
	EFI_DEVICE_PATH_PROTOCOL* BlockPath;
	CHAR16* BlockPathText;
	EFI_BLOCK_IO_PROTOCOL* OperatingSystemBlockIo;
	UINTN NumberOfEntryBlocks;
	BOOLEAN FoundFlag = FALSE;
	for (UINTN HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
		Status = gBS->HandleProtocol(BlockControllerHandles[HandleIndex], &gBlockIoProtocolGuid, (VOID**)&gBlockIoProtocol);
		if (Status != EFI_SUCCESS) {
			UefiErrorPrint(Status, L"HandleProtocol ndY27TlwP1oF");
			continue;
		}
		if (gBlockIoProtocol->Media->LogicalPartition) {
			gST->ConOut->OutputString(gST->ConOut, L"Logical partition was found.\r\n");
			continue;
		}
		Status = gBS->OpenProtocol(
			BlockControllerHandles[HandleIndex], &gDevicePathProtocolGuid, (VOID**)&BlockPath,
			ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
		if (Status != EFI_SUCCESS) {
			UefiErrorPrint(Status, L"OpenProtocol LC0775mQjjmQ");
			continue;
		}
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
		if (Status != EFI_SUCCESS) {
			UefiFree(GptHeaderBuffer);
			UefiErrorPrint(Status, L"ReadBlocks pKCsz03hpsdU");
			continue;
		}
		GptHeader = (EFI_PARTITION_TABLE_HEADER*)GptHeaderBuffer;
		if (GptHeader->Hdr.Signature != EFI_PTAB_HEADER_ID) {
			gST->ConOut->OutputString(gST->ConOut, L"GPT header was not found.\r\n");
			UefiFree(GptHeaderBuffer);
			continue;
		}
		gST->ConOut->OutputString(gST->ConOut, L"GPT header was found.\r\nNoPE: ");
		UefiPrintDecimalUnsigned(GptHeader->NumberOfPartitionEntries);
		gST->ConOut->OutputString(gST->ConOut, L"\r\nChecking partition entries...\r\n");
		NumberOfEntryBlocks = GptHeader->NumberOfPartitionEntries / 4;
		for (UINTN EntryBlockIndex = 0; EntryBlockIndex < NumberOfEntryBlocks; EntryBlockIndex++) {
			PartitionEntryBuffer = (UINT8*)UefiMalloc(gBlockIoProtocol->Media->BlockSize);
			Status = gBlockIoProtocol->ReadBlocks(
				gBlockIoProtocol, gBlockIoProtocol->Media->MediaId, (EFI_LBA)(2 + EntryBlockIndex),
				gBlockIoProtocol->Media->BlockSize, PartitionEntryBuffer);
			if (Status != EFI_SUCCESS) {
				UefiFree(PartitionEntryBuffer);
				UefiErrorPrint(Status, L"ReadBlocks pQMsYgTQyFaC");
				continue;
			}
			PartitionEntry = (EFI_PARTITION_ENTRY*)PartitionEntryBuffer;
			for (UINTN EntryDataIndex = 0; EntryDataIndex < 4; EntryDataIndex++) {
				if (IsZeroGUID(PartitionEntry[EntryDataIndex].PartitionTypeGUID)) continue;
				gST->ConOut->OutputString(gST->ConOut, L"Partition entry was found.\r\nPART");
				UefiPrintDecimalUnsigned(EntryBlockIndex * 4 + EntryDataIndex);
				gST->ConOut->OutputString(gST->ConOut, L" START:");
				UefiPrintDecimalUnsigned(PartitionEntry[EntryDataIndex].StartingLBA);
				gST->ConOut->OutputString(gST->ConOut, L" END:");
				UefiPrintDecimalUnsigned(PartitionEntry[EntryDataIndex].EndingLBA);
				gST->ConOut->OutputString(gST->ConOut, L"\r\n");
				if (EntryBlockIndex * 4 + EntryDataIndex != 1) continue;
				UefiMemCpy(OperatingSystemEntry, &PartitionEntry[EntryDataIndex], sizeof(EFI_PARTITION_ENTRY));
			}
			UefiFree(PartitionEntryBuffer);
		}
		UefiFree(GptHeaderBuffer);
		if (!FoundFlag) {
			OperatingSystemBlockIo = gBlockIoProtocol;
			FoundFlag = TRUE;
		}
	}
	gST->ConOut->OutputString(gST->ConOut, L"Checking boot signature...\r\n");
	BootRecordBuffer = (UINT8*)UefiMalloc(OperatingSystemBlockIo->Media->BlockSize);
	Status = OperatingSystemBlockIo->ReadBlocks(
		OperatingSystemBlockIo, OperatingSystemBlockIo->Media->MediaId, OperatingSystemEntry->StartingLBA,
		OperatingSystemBlockIo->Media->BlockSize, BootRecordBuffer);
	if (Status != EFI_SUCCESS) {
		UefiFree(OperatingSystemEntry);
		UefiFree(BootRecordBuffer);
		UefiErrorShutdown(Status, L"ReadBlocks SKvsx1tu1L9c");
	}
	UefiCheckSnailBootRecord((SNAILFS_BOOT_RECORD*)BootRecordBuffer, &Status);
	if (Status != EFI_SUCCESS) {
		UefiFree(OperatingSystemEntry);
		UefiFree(BootRecordBuffer);
		return Status;
	}
	gST->ConOut->OutputString(gST->ConOut, L"Checking file system...\r\n");
	TableHdrBuffer = (UINT8*)UefiMalloc(OperatingSystemBlockIo->Media->BlockSize);
	Status = OperatingSystemBlockIo->ReadBlocks(
		OperatingSystemBlockIo, OperatingSystemBlockIo->Media->MediaId, (OperatingSystemEntry->StartingLBA + 1),
		OperatingSystemBlockIo->Media->BlockSize, TableHdrBuffer);
	if (Status != EFI_SUCCESS) {
		UefiFree(OperatingSystemEntry);
		UefiFree(BootRecordBuffer);
		UefiFree(TableHdrBuffer);
		UefiErrorShutdown(Status, L"ReadBlocks 8+w5FbqmznIW");
	}
	UefiCheckSnailTableHdr((SNAILFS_TABLE_HEADER*)TableHdrBuffer, &Status);
	if (Status != EFI_SUCCESS) {
		UefiFree(OperatingSystemEntry);
		UefiFree(BootRecordBuffer);
		UefiFree(TableHdrBuffer);
		return Status;
	}
	gST->ConOut->OutputString(gST->ConOut, L"Checking other data of table header...\r\n");
	UefiCheckSnailTableSize((SNAILFS_TABLE_HEADER*)TableHdrBuffer, OperatingSystemEntry, &Status);
	if (Status != EFI_SUCCESS) {
		UefiFree(OperatingSystemEntry);
		UefiFree(BootRecordBuffer);
		UefiFree(TableHdrBuffer);
		return Status;
	}
	gST->ConOut->OutputString(gST->ConOut, L"Looking for kernel file...\r\n");
	UefiFree(OperatingSystemEntry);
	UefiFree(BootRecordBuffer);
	UefiFree(TableHdrBuffer);
	return EFI_SUCCESS;
}
