#pragma once

#include "efidef.h"

extern EFI_SYSTEM_TABLE* ST;
#define gST ST
extern EFI_BOOT_SERVICES* BS;
#define gBS BS
extern EFI_RUNTIME_SERVICES* RT;
#define gRT RT

extern EFI_GUID LoadedImageProtocolGuid;
#define gLoadedImageProtocolGuid LoadedImageProtocolGuid
extern EFI_GUID DevicePathToTextProtocolGuid;
#define gDevicePathToTextProtocolGuid DevicePathToTextProtocolGuid

extern EFI_LOADED_IMAGE_PROTOCOL* LoadedImageProtocol;
#define gLoadedImageProtocol LoadedImageProtocol
extern EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* DevicePathToTextProtocol;
#define gDevicePathToTextProtocol DevicePathToTextProtocol
