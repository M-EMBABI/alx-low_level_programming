#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * print_magic - Print the ELF magic number
 * @e_ident: ELF identification bytes
 */
void print_magic(unsigned char *e_ident)
{
    int i;

    printf("  Magic:   ");
    for (i = 0; i < EI_NIDENT; i++)
        printf("%02x%c", e_ident[i], i == EI_NIDENT - 1 ? '\n' : ' ');
}

/**
 * print_type - Print the ELF type
 * @type: ELF type
 */
void print_type(unsigned int type)
{
    const char *types[] = {
        [ET_NONE] = "NONE (Unknown type)",
        [ET_REL] = "REL (Relocatable file)",
        [ET_EXEC] = "EXEC (Executable file)",
        [ET_DYN] = "DYN (Shared object file)",
        [ET_CORE] = "CORE (Core file)",
        [ET_LOPROC] = "LOPROC (Processor-specific)",
        [ET_HIPROC] = "HIPROC (Processor-specific)",
    };

    printf("  Type:                              %s\n", types[type]);
}

/**
 * print_class - Print the ELF class
 * @class: ELF class
 */
void print_class(unsigned int class)
{
    const char *classes[] = {
        [ELFCLASSNONE] = "NONE (Unknown class)",
        [ELFCLASS32] = "ELF32",
        [ELFCLASS64] = "ELF64",
    };

    printf("  Class:                             %s\n", classes[class]);
}

/**
 * print_data - Print the ELF data encoding
 * @data: ELF data encoding
 */
void print_data(unsigned int data)
{
    const char *datas[] = {
        [ELFDATANONE] = "NONE (Unknown data format)",
        [ELFDATA2LSB] = "2's complement, little endian",
        [ELFDATA2MSB] = "2's complement, big endian",
    };

    printf("  Data:                              %s\n", datas[data]);
}

/**
 * print_version - Print the ELF version
 * @version: ELF version
 */
void print_version(unsigned int version)
{
    printf("  Version:                           %d (current)\n", version);
}

/**
 * print_osabi - Print the ELF operating system/ABI
 * @osabi: ELF operating system/ABI
 */
void print_osabi(unsigned int osabi)
{
    const char *osabis[] = {
        [ELFOSABI_NONE] = "UNIX - System V",
        [ELFOSABI_HPUX] = "HP-UX",
        [ELFOSABI_NETBSD] = "NetBSD",
        [ELFOSABI_LINUX] = "Linux",
        [ELFOSABI_SOLARIS] = "Solaris",
        [ELFOSABI_AIX] = "AIX",
        [ELFOSABI_IRIX] = "IRIX",
        [ELFOSABI_FREEBSD] = "FreeBSD",
        [ELFOSABI_TRU64] = "TRU64",
        [ELFOSABI_MODESTO] = "Novell Modesto",
        [ELFOSABI_OPENBSD] = "OpenBSD",
        [ELFOSABI_ARM_AEABI] = "ARM EABI",
        [ELFOSABI_ARM] = "ARM",
        [ELFOSABI_STANDALONE] = "Standalone (embedded) application",
    };

    printf("  OS/ABI:                            %s\n", osabis[osabi]);
}

/**
 * print_type - Print the ELF ABI version
 * @abi: ELF ABI version
 */
void print_abi_version(unsigned int abi)
{
    printf("  ABI Version:                       %u\n", abi);
}

/**
 * print_entry - Print the ELF entry point address
 * @entry: ELF entry point address
 */
void print_entry(Elf64_Addr entry)
{
    printf("  Entry point address:               0x%lx\n", entry);
}

/**
 * elf_header - Display information from the ELF header
 * @filename: Name of the ELF file
 */
void elf_header(const char *filename)
{
    int fd;
    Elf64_Ehdr header;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        dprintf(STDERR_FILENO, "Error: Unable to open file '%s'\n", filename);
        exit(98);
    }

    if (read(fd, &header, sizeof(header)) != sizeof(header))
    {
        dprintf(STDERR_FILENO, "Error: Unable to read ELF header from file '%s'\n", filename);
        exit(98);
    }

    if (header.e_ident[EI_MAG0] != ELFMAG0 ||
        header.e_ident[EI_MAG1] != ELFMAG1 ||
        header.e_ident[EI_MAG2] != ELFMAG2 ||
        header.e_ident[EI_MAG3] != ELFMAG3)
    {
        dprintf(STDERR_FILENO, "Error: '%s' is not an ELF file\n", filename);
        exit(98);
    }

    printf("ELF Header:\n");
    print_magic(header.e_ident);
    print_class(header.e_ident[EI_CLASS]);
    print_data(header.e_ident[EI_DATA]);
    print_version(header.e_ident[EI_VERSION]);
    print_osabi(header.e_ident[EI_OSABI]);
    print_abi_version(header.e_ident[EI_ABIVERSION]);
    print_type(header.e_type);
    print_entry(header.e_entry);

    close(fd);
}

int main(int ac, char **av)
{
    if (ac != 2)
    {
        dprintf(STDERR_FILENO, "Usage: %s elf_filename\n", av[0]);
        return (98);
    }

    elf_header(av[1]);
    return (0);
}
