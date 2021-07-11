#pragma once

#include <brutal/base.h>

#define MAX_MADT_RECORD_COUNT (64)

struct PACKED acpi_rsdp
{
    char signature[8];
    uint8_t checksum;
    char OEM_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
};

struct PACKED acpi_sdth
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
};

struct PACKED acpi_rsdt
{
    struct acpi_sdth base;
    uint32_t children[];
};

enum acpi_madt_record_type
{
    ACPI_MADT_RECORD_LAPIC = 0,
    ACPI_MADT_RECORD_IOAPIC = 1,
    ACPI_MADT_RECORD_MADT_ISO = 2,
    ACPI_MADT_RECORD_NMI = 4,
    ACPI_MADT_RECORD_LAPIC_OVERRIDE = 5
};

struct PACKED acpi_madt_record
{
    uint8_t type;
    uint8_t len;
};

struct PACKED acpi_madt_lapic_record
{
    struct acpi_madt_record base;

    uint8_t processor_id;
    uint8_t id;
    uint32_t flags;
};

struct PACKED acpi_madt_ioapic_record
{
    struct acpi_madt_record base;

    uint8_t id;
    uint8_t reserved;
    uint32_t address;
    uint32_t interrupt_base;
};

struct PACKED acpi_madt_iso_record
{
    struct acpi_madt_record base;

    uint8_t bus;
    uint8_t irq;             // source
    uint32_t interrupt_base; // gsi
    uint16_t flags;
};

struct PACKED acpi_madt
{
    struct acpi_sdth base;

    uint32_t local_apic;
    uint32_t flags;

    struct acpi_madt_record records[];
};

struct PACKED acpi_hpet
{
    struct acpi_sdth base;

    uint8_t hardware_rev_id;
    uint8_t info;
    uint16_t pci_vendor_id;
    uint8_t address_space_id;
    uint8_t register_bit_width;
    uint8_t register_bit_offset;
    uint8_t reserved1;
    uint64_t address;
    uint8_t hpet_number;
    uint16_t minimum_tick;
    uint8_t page_protection;
};

#define acpi_make_madt_record_table(NAME, RECORD_TYPE)    \
    struct NAME                                           \
    {                                                     \
        size_t count;                                     \
        struct RECORD_TYPE *table[MAX_MADT_RECORD_COUNT]; \
    }

acpi_make_madt_record_table(acpi_madt_record_table, acpi_madt_record);
acpi_make_madt_record_table(lapic_record_table, acpi_madt_lapic_record);
acpi_make_madt_record_table(ioapic_record_table, acpi_madt_ioapic_record);
acpi_make_madt_record_table(iso_record_table, acpi_madt_iso_record);

struct acpi_sdth *acpi_rsdt_child(struct acpi_rsdt *rsdt, Str signature);

struct acpi_madt_record *acpi_madt_record(struct acpi_madt *madt, enum acpi_madt_record_type type);

struct acpi_madt_record_table acpi_madt_multiple_record(struct acpi_madt *madt, enum acpi_madt_record_type type);

struct acpi_madt *acpi_find_madt(uintptr_t rsdp_address);

struct acpi_hpet *acpi_find_hpet(uintptr_t rsdp_address);

uint32_t acpi_find_lapic(uintptr_t rsdp_address);

struct lapic_record_table acpi_find_lapic_table(uintptr_t rsdp_address);

struct ioapic_record_table acpi_find_ioapic_table(uintptr_t rsdp_address);

struct iso_record_table acpi_find_iso_table(uintptr_t rsdp_address);
