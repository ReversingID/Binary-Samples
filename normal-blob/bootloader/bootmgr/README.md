# Binary Samples

Open repository of binary sample collections for binary analysis.

## Windows Boot Manager

bootmgr - Boot Manager.

The bootloader of Windows NT operating system. Used by windows OS from Windows Vista and newer operating system.

#### Disk & Master Boot Record

When computer boots, BIOS transfer control to the first boot device (in this case disk). The first sector is called Master Boot Record (MBR) which has 512 bytes in size. MBR contains small piece of code (446 bytes) called _primary boot loader_ and the _partition table_ (64 bytes) describing primary and extended partitions.

By default MBR code looks for the partition marked as active, loads its boot sector into memory and passes control to it.

#### Boot Sequence

Bootmgr works in stages with following sequence happen in booting process:

- **Stage 1**: located in MBR and points to _Stage 2_.

- **Stage 2**: Bootmgr switch to protected mode then loads _winload.exe_, _memtest.exe_, and _winresume.exe_. All of them are then used to load the NT kernel.