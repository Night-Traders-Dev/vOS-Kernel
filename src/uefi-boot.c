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

    // Here, you would load the kernel into memory using UEFI services.
    // You can use the UEFI File Protocol and Memory Protocol to load your kernel.

    // For simplicity, let’s assume kernel_entry() is already mapped:
    void (*kernel_entry)() = (void (*)()) 0x100000; // Replace with actual kernel entry address

    // Jump to the kernel
    kernel_entry();

    // If kernel returns, hang the system
    while (1);

    return EFI_SUCCESS;
}
