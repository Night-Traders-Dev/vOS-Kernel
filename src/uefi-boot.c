#include <efi.h>
#include <efilib.h>

// Entry point for the UEFI bootloader
EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    // Initialize the UEFI library
    InitializeLib(image_handle, system_table);

    // Print bootloader start message
    Print(L"[bootloader] Boot init completed\n");

    // Print kernel load message
    Print(L"[bootloader] Kernel init starting...\n");


    void (*kernel_entry)() = (void (*)()) 0x40000000;

    // Jump to the kernel
    kernel_entry();

    // If kernel returns, hang the system
    while (1);

    return EFI_SUCCESS;
}
