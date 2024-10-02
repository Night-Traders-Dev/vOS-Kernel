#include <efi.h>
#include <efilib.h>

// Entry point for the UEFI bootloader
EFI_STATUS EFIAPI efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    // Initialize the UEFI library
    InitializeLib(image_handle, system_table);

    // Print bootloader start message
    Print(L"[bootloader] Boot init completed\n");

    // Print kernel load message
    Print(L"[bootloader] Kernel init starting...\n");

    // Define kernel entry point
    void (*kernel_entry)(void) = (void (*)(void)) 0x40000000;

    // Jump to the kernel
    kernel_entry();

    // Infinite loop to hang the system if kernel returns
    while (1);

    return EFI_SUCCESS; // This line is never reached, but kept for clarity
}
