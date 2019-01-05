# Binary Samples

Open repository of binary sample collections for binary analysis.

## GRUB (Bootloader)

GRUB - GRand Unified Bootloader.

Capable of loading a variety of free and proprietary operating system. Works well with Linux, DOS, Windows, or BSD on PC (x86 / x64 system).

#### Disk & Master Boot Record

When computer boots, BIOS transfer control to the first boot device (in this case disk). The first sector is called Master Boot Record (MBR) which has 512 bytes in size. MBR contains small piece of code (446 bytes) called _primary boot loader_ and the _partition table_ (64 bytes) describing primary and extended partitions.

By default MBR code looks for the partition marked as active, loads its boot sector into memory and passes control to it.

GRUB replace default MBR with its own code.

#### GRUB Stages

GRUB works in stages.

**Stage 1**: located in MBR, and mainly points to _Stage 2_.

**Stage 2**: points to configuration file (i.e. grub.cfg), which contains all the complex user interface and options.

**Stage 1.5**: might be exists if the boot information is small enough to fit in the area immediately after MBR.