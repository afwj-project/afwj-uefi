#pragma once

#include "efidef.h"
#include "efifpt.h"

/// @brief Memory address for sharing operating system block I/O protocol
#define OSBI_SHARED_ADDRESS ((EFI_BLOCK_IO_PROTOCOL*)0x0000000002000000)
/// @brief Memory address for sharing simple text output protocol
#define STOP_SHARED_ADDRESS ((EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL*)0x0000000004000000)

extern EFI_SYSTEM_TABLE* ST;
#define gST ST
extern EFI_BOOT_SERVICES* BS;
#define gBS BS
extern EFI_RUNTIME_SERVICES* RT;
#define gRT RT

extern EFI_GUID LoadedImageProtocolGuid;
#define gLoadedImageProtocolGuid LoadedImageProtocolGuid
extern EFI_GUID DevicePathProtocolGuid;
#define gDevicePathProtocolGuid DevicePathProtocolGuid
extern EFI_GUID DevicePathToTextProtocolGuid;
#define gDevicePathToTextProtocolGuid DevicePathToTextProtocolGuid
extern EFI_GUID DevicePathFromTextProtocolGuid;
#define gDevicePathFromTextProtocolGuid DevicePathFromTextProtocolGuid
extern EFI_GUID DevicePathUtilitiesProtocolGuid;
#define gDevicePathUtilitiesProtocolGuid DevicePathUtilitiesProtocolGuid
extern EFI_GUID DiskIoProtocolGuid;
#define gDiskIoProtocolGuid DiskIoProtocolGuid
extern EFI_GUID DiskIo2ProtocolGuid;
#define gDiskIo2ProtocolGuid DiskIo2ProtocolGuid
extern EFI_GUID BlockIoProtocolGuid;
#define gBlockIoProtocolGuid BlockIoProtocolGuid
extern EFI_GUID BlockIo2ProtocolGuid;
#define gBlockIo2ProtocolGuid BlockIo2ProtocolGuid

extern EFI_LOADED_IMAGE_PROTOCOL* LoadedImageProtocol;
#define gLoadedImageProtocol LoadedImageProtocol
extern EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* DevicePathToTextProtocol;
#define gDevicePathToTextProtocol DevicePathToTextProtocol
extern EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL* DevicePathFromTextProtocol;
#define gDevicePathFromTextProtocol DevicePathFromTextProtocol
extern EFI_DEVICE_PATH_UTILITIES_PROTOCOL* DevicePathUtilitiesProtocol;
#define gDevicePathUtilitiesProtocol DevicePathUtilitiesProtocol
extern EFI_DISK_IO_PROTOCOL* DiskIoProtocol;
#define gDiskIoProtocol DiskIoProtocol
extern EFI_BLOCK_IO_PROTOCOL* BlockIoProtocol;
#define gBlockIoProtocol BlockIoProtocol

extern EFI_PARTITION_ENTRY* OperatingSystemEntry;
#define gOperatingSystemEntry OperatingSystemEntry
extern EFI_BLOCK_IO_PROTOCOL* OperatingSystemBlockIo;
#define gOperatingSystemBlockIo OperatingSystemBlockIo
